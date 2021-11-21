#include "TcpClient.h"
#include "main.h"
#include "string.h"
#include "parson.h"

extern RTC_HandleTypeDef hrtc;

void TcpClient::init() {
	struct tcp_pcb *control_block = tcp_new();
	ip_addr_t destIp;
	IP_ADDR4(&destIp, 213, 188, 196, 246);  // 213.188.196.246
	tcp_connect(control_block, &destIp, 80, connectedCallback);
}

err_t TcpClient::connectedCallback(void* arg, tcp_pcb *controlBlock, err_t err) {
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  tcpClientDetails* clientDetails = (tcpClientDetails*)mem_malloc(sizeof(tcpClientDetails));
  if (clientDetails != NULL) {
    clientDetails->state = CONNECTED;
    clientDetails->controlBlock = controlBlock;
    clientDetails->retries = 0;
    clientDetails->buffer = NULL;

    tcp_arg(controlBlock, clientDetails);

    tcp_recv(controlBlock, recvCallback);  // attach callbacks
    tcp_poll(controlBlock, pollCallback, 0);
    tcp_sent(controlBlock, ackCallback);

    char request[] = "GET /api/timezone/Europe/Warsaw HTTP/1.1\nConnection: Close\n\n";
	int request_len = strlen(request);
	clientDetails->buffer = pbuf_alloc(PBUF_TRANSPORT, request_len, PBUF_POOL);
	pbuf_take(clientDetails->buffer, request, request_len);
	send(clientDetails);
	pbuf_free(clientDetails->buffer);

    return ERR_OK;
  } else {
    disconnect(controlBlock, clientDetails);
    return ERR_MEM;
  }
}

err_t TcpClient::recvCallback(void *arg, tcp_pcb *controlBlock, pbuf *buffer, err_t err) {
  tcpClientDetails* clientDetails = (tcpClientDetails*) arg;

  if (buffer == NULL) {  // empty datagram = disconnect
    clientDetails->state = DISCONNECTING;
    disconnect(controlBlock, clientDetails);
    return ERR_OK;
  } else if (err != ERR_OK) { // non empty datagram received but error occured
    if (buffer != NULL) {
      clientDetails->buffer = NULL;
      pbuf_free(buffer);
    }
    return err;
  } else if (clientDetails->state == CONNECTED) {
    clientDetails->buffer = buffer;
    tcp_recved(controlBlock, buffer->tot_len);  // send ack for received data
    handle(controlBlock, clientDetails);
    pbuf_free(buffer);
    return ERR_OK;
  } else {  // something strange happened
    tcp_recved(controlBlock, buffer->tot_len);
    clientDetails->buffer = NULL;
    pbuf_free(buffer);
    return ERR_OK;
  }
}

err_t TcpClient::pollCallback(void* arg, struct tcp_pcb *controlBlock) {
  tcpClientDetails* clientDetails = (tcpClientDetails*) arg;
  if (clientDetails != NULL) {
    if (clientDetails->buffer == NULL && clientDetails->state == DISCONNECTING) {
        disconnect(controlBlock, clientDetails);
    }
    return ERR_OK;
  } else {
    tcp_abort(controlBlock);
    return ERR_ABRT;
  }
}

err_t TcpClient::ackCallback(void* arg, tcp_pcb *controlBlock, u16_t len) {
  LWIP_UNUSED_ARG(len);

  tcpClientDetails* clientDetails = (tcpClientDetails*) arg;
  clientDetails->retries = 0;

  if (clientDetails->buffer == NULL && clientDetails->state == DISCONNECTING) {
      disconnect(controlBlock, clientDetails);
  }
  return ERR_OK;
}

void TcpClient::send(tcpClientDetails* clientDetails) {
  struct pbuf *buffer;
  err_t err = ERR_OK;

  while (err == ERR_OK && clientDetails->buffer != NULL && clientDetails->buffer->len <= tcp_sndbuf(clientDetails->controlBlock)) {
    buffer = clientDetails->buffer;
    err = tcp_write(clientDetails->controlBlock, buffer->payload, buffer->len, 1);

    if (err == ERR_OK) {
      clientDetails->buffer = buffer->next;

      if (clientDetails->buffer != NULL) {
        pbuf_ref(clientDetails->buffer);
      }

      u8_t freed;
      do {
        freed = pbuf_free(buffer);
      } while(freed == 0);
   } else if (err == ERR_MEM) {
     clientDetails->buffer = buffer;
   }
  }
}

void TcpClient::disconnect(tcp_pcb* controlBlock, tcpClientDetails* clientDetails) {
  tcp_arg(controlBlock, NULL);  // remove callbacks
  tcp_sent(controlBlock, NULL);
  tcp_recv(controlBlock, NULL);
  tcp_err(controlBlock, NULL);
  tcp_poll(controlBlock, NULL, 0);

  if (clientDetails != NULL) {
    mem_free(clientDetails);
  }

  tcp_close(controlBlock);
}

void TcpClient::handle(tcp_pcb* controlBlock, tcpClientDetails* clientDetails) {
	static char res[2000];
	static int res_length = 0;

	char b[1000];
	sprintf(b, "%s", (char*)(clientDetails->buffer->payload));
	if (strncmp(b, "HTTP", (unsigned int)4) == 0) {
		strcpy(res, b);
		res_length = clientDetails->buffer->len;
	} else {
		for (int i=0; i<clientDetails->buffer->len; i++) {
			res[res_length+i] = b[i];
		}
		res_length += clientDetails->buffer->len;

		char* json = strstr(res, "\r\n\r\n") + 4;

		JSON_Value *data = json_parse_string(json);
		const char* datetime = json_object_get_string(json_object(data), "datetime");
		const char* day_of_week = json_object_get_string(json_object(data), "day_of_week");

		char val[5];
		memcpy(val, datetime, 4);
		val[4] = '\0';
		int year = atoi(val);

		memcpy(val, &datetime[5], 2);
		val[2] = '\0';
		int month = atoi(val);

		memcpy(val, &datetime[8], 2);
		val[2] = '\0';
		int day = atoi(val);

		memcpy(val, &datetime[11], 2);
		val[2] = '\0';
		int hour = atoi(val);

		memcpy(val, &datetime[14], 2);
		val[2] = '\0';
		int minute = atoi(val);

		memcpy(val, &datetime[17], 2);
		val[2] = '\0';
		int second = atoi(val);

		int i_day_of_week = atoi(day_of_week);

		RTC_TimeTypeDef sTime = {0};
		RTC_DateTypeDef sDate = {0};

		sTime.Hours = hour;
		sTime.Minutes = minute;
		sTime.Seconds = second;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}
		sDate.WeekDay = i_day_of_week;
		sDate.Month = month;
		sDate.Date = day;
		sDate.Year = year-2000;
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}
	}
}
