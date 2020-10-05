/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "tusb.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]         HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_config_report[] =
{
  0x06, 0x01, 0x00,		// USAGE_PAGE (Generic Desktop)
  0x09, 0x01,			// USAGE (Vendor Usage 1)
  0xA1, 0x01,			// COLLECTION (Application)
  0x15, 0x00			// LOGICAL_MINIMUM (0)
  0x26, 0xFF, 0x00,		// LOGICAL_MAXIMUM (255)
  0x75, 0x08,			// REPORT_SIZE (8)
  0x95, 0x15,			// REPORT_COUNT (21)
  0x81, 0x00,			// INPUT (Data,Ary,Abs)
  0x91, 0x00,			// OUTPUT (Data,Ary,Abs)
  0xC0					// END_COLLECTION
};

uint8_t desc_keyboard_report[] =
{
  0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
  0x09, 0x06,			// USAGE (Keyboard)
  0xA1, 0x01,			// COLLECTION (Application)
  0x05, 0x07,			// USAGE_PAGE (Keyboard)
  0x19, 0xE0,			// USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xE7,			// USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,			// LOGICAL_MINIMUM (0)
  0x25, 0x01,			// LOGICAL_MAXIMUM (1)
  0x75, 0x01,			// REPORT_SIZE (1)
  0x95, 0x08,			// REPORT_COUNT (8)
  0x81, 0x02,			// INPUT (Data,Var,Abs)
  0x75, 0x08,			// REPORT_SIZE (8)
  0x95, 0x01,			// REPORT_COUNT (1)
  0x81, 0x01,			// INPUT (Cnst,Ary,Abs)
  0x75, 0x01,			// REPORT_SIZE (1)
  0x95, 0x05,			// REPORT_COUNT (5)
  0x05, 0x08,			// USAGE_PAGE (LEDS)
  0x19, 0x01,			// USAGE_MINIMUM (Num Lock)
  0x29, 0x05,			// USAGE_MAXIMUM (Kana)
  0x91, 0x02,			// OUTPUT (Data,Var,Abs)
  0x75, 0x03,			// REPORT_SIZE (3)
  0x95, 0x01,			// REPORT_COUNT (1)
  0x91, 0x01,			// OUTPUT (Cnst,Ary,Abs)
  0x75, 0x08,			// REPORT_SIZE (8)
  0x95, 0x09,			// REPORT_COUNT (9)
  0x05, 0x07,			// USAGE_PAGE (Keyboard)
  0x15, 0x00,			// LOGICAL_MINIMUM (0)
  0x25, 0x65,			// LOGICAL_MAXIMUM (101)
  0x19, 0x00,			// USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,			// USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,			// INPUT (Data,Ary,Abs)
  0xC0					// END_COLLECTION
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t desc_index)
{
  if (desc_index == 0)
	{
		return desc_config_report;
	}
	else if (desc_index == 1)
	{
		return desc_keyboard_report;
	}

		return 0;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum
{
  ITF_HID_CONFIG,
  ITF_HID_KEYBOARD,
  ITF_NUM_TOTAL
};

#define TUD_HID_CONFIG_DESC_LEN (21)
#define TUD_HID_KEYBOARD_DESC_LEN (63)
#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_CONFIG_DESC_LEN + TUD_HID_KEYBOARD_DESC_LEN)

#define EPNUM_CONFIG   0x01
#define EPNUM_KEYBOARD   0x02

uint8_t const desc_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

	//@TODO FIX THIS TO MATCH NEW REPORT DESCRIPTOR
  // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
  TUD_HID_DESCRIPTOR(ITF_HID_CONFIG, 4, HID_PROTOCOL_NONE, sizeof(desc_hid_report1), EPNUM_HID1, CFG_TUD_HID_EP_BUFSIZE, 10),
  TUD_HID_DESCRIPTOR(ITF_NUM_HID2, 5, HID_PROTOCOL_NONE, sizeof(desc_hid_report2), EPNUM_HID2, CFG_TUD_HID_EP_BUFSIZE, 10)
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations
  return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 },	// 0: is supported language is English (0x0409)
  "Mechanical Squid Factory",		// 1: Manufacturer
  "Macro9Pad",						// 2: Product
  "123456",							// 3: Serials, should use chip ID
  "Configuration Interface",		// 4: Interface 1 String
  "Keyboard Interface",				// 5: Interface 2 String
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}
