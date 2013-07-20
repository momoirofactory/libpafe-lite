/* $Id: pasori_command.c,v 1.11 2009-10-09 07:43:13 hito Exp $ */
/* pasori commands */
#ifdef HAVE_CONFIG_H 
#include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <usb.h>
#include "libpafe.h"

// SONY Felica Reader
#define PASORIUSB_VENDOR 0x054c
#define PASORIUSB_PRODUCT_S310 0x006c
#define PASORIUSB_PRODUCT_S320 0x01bb
#define PASORIUSB_PRODUCT_S330 0x02e1

// SCR331CL-NTTCom
#define PASORIUSB_VENDOR_SCR331CL 0x04e6
#define PASORIUSB_PRODUCT_SCR331CL 0x5290

#define TIMEOUT 200
//#define TIMEOUT 100


#if DEBUG_USB
static int sUsbDebugLevel = 255; // debug
#else
static int sUsbDebugLevel = 0; // release
#endif


/* FIXME: UNKNOWN CONSTANTS */
static const uint8 S320_INIT0[] = { 0x62, 0x01, 0x82 };
/* RET0		{0x63,0x00,0x88}; */

static const uint8 S320_INIT1[] = { 0x62, 0x02, 0x80, 0x81 };	/* INIT3 */
/* RET1		{0x63,0x00,0xcc,0x88}; */

static const uint8 S320_INIT2[] = { 0x62, 0x22, 0x80, 0xcc, 0x81, 0x88 };
/* RET2		{0x63,0x00}; */

static const uint8 S320_INIT3[] = { 0x62, 0x02, 0x80, 0x81 };	/* INIT1 */
/* RET3		{0x63,0x00,0xcc,0x88}; */

static const uint8 S320_INIT4[] = { 0x62, 0x02, 0x82, 0x87 };
/* RET4		{0x63,0x00,0x88,0x01}; */

static const uint8 S320_INIT5[] = { 0x62, 0x21, 0x25, 0x58 };
/* RET5		{0x63,0x00} */

static const uint8 S320_READ0[] = { 0x58 };
/*RRET0		{0x59,0x28,0x01} */

static const uint8 S320_READ1[] = { 0x54 };
static const uint8 S320_READ2[] = { 0x5a, 0x80 };

static const uint8 S310_INIT[] = { 0x54 };

static const uint8 S330_RF_ANTENNA_ON[] = { 0xD4, 0x32, 0x01, 0x01 };
static const uint8 S330_RF_ANTENNA_OFF[] = { 0xD4, 0x32, 0x01, 0x00 };
static const uint8 S330_GET_VERSION[] = { 0xD4, 0x02 };
static const uint8 S330_DESELECT[] = { 0xD4, 0x44, 0x01 };

static void _log(char* message, ...);


#define DATASIZE 255

/* internal */

static int checksum(uint8 *data, int size) {
	int i, sum = 0;

	if (data == NULL)
		return 0;

	for (i = 0; i != size; i++) {
		sum += data[i];
	}
	sum &= 0xff;
	sum = 0x100 - sum;

	return sum & 0xff;
}

static int pasori_init_test(pasori *p, const uint8 *testptrn, int size) {
	uint8 recv[DATASIZE + 1];
	int n, r;

	if (p == NULL || testptrn == NULL || size < 1)
		return PASORI_ERR_PARM;

	n = size;
	r = pasori_packet_write(p, (uint8 *) testptrn, &n);
	if (r)
		return r;
	n = DATASIZE;
	r = pasori_recv(p, recv, &n);

	return r;
}

/* exports */

int pasori_test(pasori *p, int code, uint8 *data, int *size, uint8 *rdata, int *rsize) {
	uint8 recv[DATASIZE + 1];
	int n, r;


	if (p == NULL || size == NULL)
		return PASORI_ERR_PARM;

	if (code == 0x00 && (size ==NULL ||rdata == NULL || rsize == NULL))
		return PASORI_ERR_PARM;

	switch (p->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		break;
	default:
		return PASORI_ERR_TYPE;
	}

	n = *size;

	if (n > DATASIZE - 3)
		return PASORI_ERR_PARM;

	recv[0] = 0x52;
	recv[1] = code;
	recv[2] = *size;
	if (n > 0) {
		memcpy(recv + 3, data, n);
	}
	n += 3;

	r = pasori_packet_write(p, recv, &n);
	if (r)
		return r;

	n = DATASIZE;
	r = pasori_packet_read(p, recv, &n);
	if (r)
		return r;

	if (recv[0] != 0x53)
		return PASORI_ERR_FORMAT;

	n = recv[1];
	if (code != 0x00 && n != 1) {
		return n;
	}

	if (code != 0x00)
		return 0;

	if (n > *rsize)
		n = *rsize;

	recv[2 + n] = '\0';
	memcpy(rdata, &recv[2], n);
	*rsize = n;
	return 0;
}

int pasori_test_echo(pasori *p, uint8 *data, int *size) {
	int n = *size, l = DATASIZE, r;
	uint8 rdata[DATASIZE + 1];

	r = pasori_test(p, 0x00, data, &n, rdata, &l);
	if (r)
		return r;

	if (n != l)
		return PASORI_ERR_DATA;

	if (memcmp(data, rdata, n))
		return PASORI_ERR_DATA;

	return 0;
}

int pasori_test_eprom(pasori *p) {
	uint8 recv[DATASIZE + 1];
	int n = 0, rn = DATASIZE;
	return pasori_test(p, 0x01, NULL, &n, recv, &rn);
}

int pasori_test_ram(pasori *p) {
	int n = 0;
	return pasori_test(p, 0x02, NULL, &n, NULL, NULL);
}

int pasori_test_cpu(pasori *p) {
	int n = 0;
	return pasori_test(p, 0x03, NULL, &n, NULL, NULL);
}

int pasori_test_polling(pasori *p) {
	int n = 0;
	return pasori_test(p, 0x10, NULL, &n, NULL, NULL);
}


void pasori_set_timeout(pasori *p, int timeout) {
	if (p == NULL || timeout < 0)
		return;

	p->timeout = timeout;
}

// RAW Packet SEND
int pasori_packet_write(pasori *p, uint8 *data, int *size) {
	uint8 cmd[DATASIZE + 1];
	uint8 sum;
	int i, n;

	if (p == NULL || data == NULL || size == NULL)
		return PASORI_ERR_PARM;

	n = *size;

	if (n < 1) {
		*size = 0;
		return 0;
	}

	if (n > DATASIZE - 7)
		n = DATASIZE - 7;

	sum = checksum(data, n);

	cmd[0] = 0;
	cmd[1] = 0;;
	cmd[2] = 0xff;
	cmd[3] = n;
	cmd[4] = 0x100 - n;
	memcpy(cmd + 5, data, n);
	cmd[5 + n] = sum;
	cmd[6 + n] = 0;
	n += 7;

	i = pasori_send(p, cmd, &n);

	*size = n - 7;

	return i;
	/* FIXME:handle error */
}

int pasori_packet_read(pasori * p, uint8 * data, int *size) {
	uint8 recv[DATASIZE + 1];
	unsigned int s;
	int i, n, sum;

	if (p == NULL || data == NULL || size == NULL)
		return PASORI_ERR_PARM;

	if (*size < 1) {
		*size = 0;
		return 0;
	}

	n = DATASIZE;
	i = pasori_recv(p, recv, &n);

	if (i)
		return i;			/* FIXME: handle timeout */

	if (recv[0] != 0 || recv[1] != 0 || recv[2] != 0xff)
		return PASORI_ERR_COM;

	if (recv[5] == 0x7f)
		return PASORI_ERR_FORMAT;

	s = recv[3];
	if (recv[4] != 0x100 - s)
		return PASORI_ERR_CHKSUM;

	sum = checksum(recv + 5, s);
	if (recv[s + 5] != sum)
		return PASORI_ERR_CHKSUM;

	if (recv[s + 6] != 0)
		return PASORI_ERR_COM;

	if (s > n)
		s = n;

	memcpy(data, &recv[5], s);
	*size = s;

	return 0;
}

/*
 * 3th byte is the max number of target to sence.	
 * It seems valid values are 0x01 or 0x02 for devices this driver suport, and
 * it can sences two targets at Mifare cards, but not at Felica cards.	
 * 
 * 4th byte may mean protocol(tag types).
 *	 0x00 = mifare,iso14443A(106kbps*)
 *	 0x01 = Felica(212kbps)
 *	 0x02 = Felica(424kbps)
 *	 0x03 = iso14443B(106kbps*)
 *	 0x04 = Nfc Forum Type1(106kbps*)
 *	 *=default bit rate(To change this, use pn53x 'D44E' command.)
 * 
 * At least on default setting, it seems my pn531 dose not accept '0x03''0x04'.
 * RC-S330 accepts '0x03''0x04', but in the '0x03' case payload length is up to 
 * 2 byte, in the '0x04' case payload length is zero , and in both case
 * 3th byte must be '0x01'.
 * 
 * */
int pasori_list_passive_target(pasori *pp, unsigned char *payload, int *size) {
	int r, n;
	unsigned char cmd[DATASIZE + 1];

	if(pp == NULL || payload == NULL || size == NULL || *size < 0) 
		return PASORI_ERR_FORMAT;

	if (pp->type != PASORI_TYPE_S330)
		return PASORI_ERR_TYPE;

	cmd[0] = 0xd4;
	cmd[1] = 0x4a;
	cmd[2] = 1;
	cmd[3] = 0x01;

	n = *size;
	memcpy(cmd + 4, payload, n);
	n += 4;
	r = pasori_packet_write(pp, cmd, &n);
	*size = n - 4;

	return r;			/* FIXME:handle error */
}

int pasori_write(pasori *p, uint8 *data, int *size) {
	uint8 cmd[DATASIZE];
	int r, n, head_len;

	n = *size;

	if (n > DATASIZE - 2)
		return PASORI_ERR_PARM;

	switch (p->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		cmd[0] = 0x5c;
		cmd[1] = *size + 1;
		head_len = 2;
		break;
	case PASORI_TYPE_S330:
		cmd[0] = 0xd4;
		cmd[1] = 0x42;
		cmd[2] = *size + 1;
		head_len = 3;
		break;
	}

	memcpy(cmd + head_len, data, n);
	n += head_len;
	r = pasori_packet_write(p, cmd, &n);
	*size = n - head_len;

	return r;			/* FIXME:handle error */
}

int pasori_read(pasori *p, uint8 *data, int *size) {
	uint8 recv[DATASIZE + 1];
	int s;
	int n, r;

	if (p == NULL || data == NULL || size == NULL)
		return PASORI_ERR_PARM;

	if (*size < 1) {
		*size = 0;
		return 0;
	}

	n = DATASIZE;
	r = pasori_packet_read(p, recv, &n);
	if (r) {
		return r;
	}

	switch (p->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		if (recv[0] != 0x5d)
			return PASORI_ERR_FORMAT;
		s = recv[1];
		break;
	case PASORI_TYPE_S330:
		if (recv[0] != 0xd5)
			return PASORI_ERR_FORMAT;
		s = n;
		break;
	}

	if (s > *size)
		s = *size;

	memcpy(data, recv + 2, s);
	*size = s;

	return 0;
}

int pasori_init(pasori * p) {
	if (p == NULL)
		return PASORI_ERR_PARM;

	switch (p->type) {
	case PASORI_TYPE_S310:
		pasori_init_test(p, S310_INIT, sizeof(S310_INIT));
		break;
	case PASORI_TYPE_S320:
		pasori_init_test(p, S320_INIT0, sizeof(S320_INIT0));
		pasori_init_test(p, S320_INIT1, sizeof(S320_INIT1));
		pasori_init_test(p, S320_INIT2, sizeof(S320_INIT2));
		pasori_init_test(p, S320_INIT3, sizeof(S320_INIT3));
		pasori_init_test(p, S320_INIT4, sizeof(S320_INIT4));
		pasori_init_test(p, S320_INIT5, sizeof(S320_INIT5));

		pasori_init_test(p, S320_READ2, sizeof(S320_READ2));
		break;
	case PASORI_TYPE_S330:
		pasori_init_test(p, S330_RF_ANTENNA_ON, sizeof(S330_RF_ANTENNA_ON));
		break;
	}
	return 0;
}

int pasori_reset(pasori * p) {
	if (p == NULL)
		return PASORI_ERR_PARM;

	switch (p->type) {
	case PASORI_TYPE_S310:
		pasori_init_test(p, S310_INIT, sizeof(S310_INIT));
		break;
	case PASORI_TYPE_S320:
		pasori_init_test(p, S320_READ1, sizeof(S320_READ1));
		break;
	case PASORI_TYPE_S330:
		pasori_init_test(p, S330_DESELECT, sizeof(S330_DESELECT));
		pasori_init_test(p, S330_RF_ANTENNA_OFF, sizeof(S330_RF_ANTENNA_ON));
		break;
	}

	return 0;
}

static int bcd2int(uint8 bcd) {
	return ((bcd >> 4) & 0x0f) * 10 + (bcd & 0x0f);
}

int pasori_version(pasori *p, int *v1, int *v2) {
	uint8 recv[DATASIZE + 1];
	int n, r, ofst;

	if (p == NULL || v1 == NULL || v2 == NULL)
		return PASORI_ERR_PARM;

	switch (p->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		recv[0] = 0x58;
		n = 1;
		ofst = 0;
		break;
	case PASORI_TYPE_S330:
		recv[0] = 0xd4;
		recv[1] = 0x02;
		n = 2;
		ofst = 2;
		break;
	}

	r = pasori_packet_write(p, recv, &n);
	if (r)
		return r;

	n = DATASIZE;
	r = pasori_packet_read(p, recv, &n);
	if (r)
		return r;

	switch (p->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		if (recv[0] != 0x59)
			return PASORI_ERR_FORMAT;
		*v1 = recv[2];
		*v2 = recv[1];
		break;
	case PASORI_TYPE_S330:
		*v1 = bcd2int(recv[3]);
		*v2 = bcd2int(recv[4]);
		break;
	}

	return 0;
}

static void dbg_dump(unsigned char *b, uint8 size) {
	int i;

	for (i = 0; i != size; i++) {
		_log("%02X ", b[i]);
	}
	_log("\n");
}

void pasori_close(pasori * pp) {
	if (!pp)
		return;

	pasori_reset(pp);
	usb_release_interface(pp->dh,
			pp->dev->config->interface->altsetting->
			bInterfaceNumber);
	usb_close(pp->dh);
	free(pp);
}

static void get_end_points(pasori *pas) {
	int uiIndex;
	int uiEndPoint;
	struct usb_interface_descriptor* puid = pas->dev->config->interface->altsetting;

	// 3 Endpoints maximum: Interrupt In, Bulk In, Bulk Out
	for(uiIndex = 0; uiIndex < puid->bNumEndpoints; uiIndex++) {
		// Only accept bulk transfer endpoints (ignore interrupt endpoints)
		if(puid->endpoint[uiIndex].bmAttributes != USB_ENDPOINT_TYPE_BULK) continue;

		// Copy the endpoint to a local var, makes it more readable code
		uiEndPoint = puid->endpoint[uiIndex].bEndpointAddress;

		// Test if we dealing with a bulk IN endpoint
		if((uiEndPoint & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_IN) {
			_log("Bulk endpoint in	: 0x%02X\n", uiEndPoint);
			pas->ep_in = uiEndPoint;
		}

		// Test if we dealing with a bulk OUT endpoint
		if((uiEndPoint & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_OUT) {
			_log("Bulk endpoint out	: 0x%02X\n", uiEndPoint);
			pas->ep_out = uiEndPoint;
		}
	}
}

pasori *
pasori_open(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	pasori *pp;

	pp = (pasori *) malloc(sizeof(pasori));

	if (pp == NULL)
		return NULL;

	usb_init();

	usb_set_debug(sUsbDebugLevel);

	usb_find_busses();

	// http://libusb.sourceforge.net/doc/function.usbfinddevices.html
	usb_find_devices();

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			_log("!CHECK for %04x:%04x\n", dev->descriptor.idVendor, dev->descriptor.idProduct);
			if (dev->descriptor.idVendor == PASORIUSB_VENDOR &&
		(dev->descriptor.idProduct == PASORIUSB_PRODUCT_S310 ||
		 dev->descriptor.idProduct == PASORIUSB_PRODUCT_S320 ||
		 dev->descriptor.idProduct == PASORIUSB_PRODUCT_S330)) {
	_log("!Device is found %04x:%04x\n", dev->descriptor.idVendor, dev->descriptor.idProduct);
	goto finish;
			}
		}
	}
	free(pp);
	_log("pasori not found in USB BUS");
	return NULL;

 finish:
	switch (dev->descriptor.idProduct) {
	case PASORIUSB_PRODUCT_S310:
		pp->type = PASORI_TYPE_S310;
		break;
	case PASORIUSB_PRODUCT_S320:
		pp->type = PASORI_TYPE_S320;
		break;
	case PASORIUSB_PRODUCT_S330:
		pp->type = PASORI_TYPE_S330;
		break;
	default:
		free(pp);
		return NULL;
	}

	pp->dh = usb_open(dev);
	pp->dev = dev;
	pp->timeout = TIMEOUT;
	get_end_points(pp);

	if (usb_set_configuration(pp->dh, 1)) {
		/* error */
		pasori_close(pp);
		return NULL;
	}

	if (usb_claim_interface
			(pp->dh, pp->dev->config->interface->altsetting->bInterfaceNumber)) {
		/* error */
		pasori_close(pp);
		return NULL;
	}
	return pp;
}

int
pasori_send(pasori *pp, uint8 *data, int *size)
{
	uint8 resp[256];
	signed int i;

	if (pp == NULL || data == NULL || size == NULL)
		return PASORI_ERR_PARM;

	if (*size < 1)
		return 0;

	_log("(send) send:");
	dbg_dump(data, *size);

	switch (pp->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		i = usb_control_msg(pp->dh, USB_TYPE_VENDOR, 0, 0, 0, data, *size, pp->timeout);
		break;
	case PASORI_TYPE_S330:
		i = usb_bulk_write(pp->dh, pp->ep_out, data, *size, pp->timeout);
		break;
	}

	if (i < 0)
		return PASORI_ERR_COM;			/* FIXME:HANDLE INVALID RESPONSES */

	*size = i;

	switch (pp->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		i = usb_interrupt_read(pp->dh, 0x81, resp, sizeof(resp), pp->timeout);
		break;
	case PASORI_TYPE_S330:
		i = usb_bulk_read(pp->dh, pp->ep_in, resp, sizeof(resp), pp->timeout);
		break;
	}

	if (i < 0)
		return PASORI_ERR_COM;			/* FIXME:HANDLE INVALID RESPONSES */

	if (i != 6)
		return PASORI_ERR_DATA;

	if (resp[4] != 0xff)
		return PASORI_ERR_DATA;

	_log("(ACK?) recv:");
	dbg_dump(resp, i);

	return 0;

}

int
pasori_recv(pasori *pp, uint8 *data, int *size)
{
	signed int i;

	if (pp == NULL || data == NULL || size == NULL)
		return 1;

	if (*size < 1)
		return 0;

	switch (pp->type) {
	case PASORI_TYPE_S310:
	case PASORI_TYPE_S320:
		i = usb_interrupt_read(pp->dh, 0x81, data, *size, pp->timeout);
		break;
	case PASORI_TYPE_S330:
		i = usb_bulk_read(pp->dh, pp->ep_in, data, *size, pp->timeout);
		break;
	}
	if (i < 0) {
		_log("(recv) ERROR\n");
		return PASORI_ERR_COM;
	}

	_log("(recv) recv:");
	dbg_dump(data, i);
	*size = i;

	return 0;
}

static void
_log(char* message, ...) {
	va_list args;

#if	DEBUG_USB
	//vprintf(message, args);
	return;
#endif

#if	DEBUG
	//vprintf(message, args);
#endif
}

