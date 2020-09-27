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
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = 0x01,	//Device TYPE
    .bcdUSB             = 0x0200,	//USB Spec Release 2.00
    .bDeviceClass       = 0x03,	//Base Class HID
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = 8,

    .idVendor           = 0x1209,	// PID.Codes VID
    .idProduct          = 0x9001,	// TODO Still need to register this
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

uint8_t const desc_hid_report[] =
{
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(void)
{
  return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_configuration[] =
{
	// Configuration Descriptor
	0x09,		// bLength
	0x02,		// bDescriptorType (Configuration)
	0x29, 0x00	// bTotalLength (this is 41 but recalculate this)
	0x02,		// bNumInterfaces
	0x01,		// bConfigurationValue
	0x00,		// iConfiguration
	0xA0,		// bmAttributes (Bus Powered, Remote Wakeup)
	0x32,		// bMaxPower (2mA units x 50 units = 100mA)
	
	/*
	 ****************************************
	 * CONFIG INTERFACE
	 ****************************************
	*/
	
	// First Interface Descriptor (Config Interface)
	0x09,		// bLength
	0x04,		// bDescriptorType (interface)
	0x00,		// bInterfaceNumber (Interface 0)
	0x00,		// bAlternateSetting
	0x02,		// bNumEndpoints
	0x03,		// bInterfaceClass (3 HID)
	0x00,		// bInterfaceSubclass
	0x00,		// bInterfaceProtocol
	0x00,		// bInterface
	
	// HID Descriptor
	0x09,		// bLength
	0x21,		// bDescriptorType (HID)
	0x10, 0x01,	// bcdHID (V1.1)
	0x00,		// bCountryCode
	0x01,		// bNumDescriptors
	0x22,		// bDescriptorType (Still a report descriptor left)
	0x2f, 0x00,	// bDescriptorLength (47 bytes)
	
	// EP1 IN Endpoint Descriptor
	0x07,		// bLength
	0x05,		// bDescriptorType (endpoint)
	0x81,		// bEndpointAddress (EP1 IN)
	0x03,		// bmAttributes (Interrupt Transfer)
	0x20, 0x00,	// wMaxPacketSize (TODO need to update this)
	0x05,		// bInterval (5ms can be a bit slower for this interface)
	
	// EP1 OUT Endpoint Descriptor
	0x07,		// bLength
	0x05,		// BDescriptorType
	0x01,		// bEndpointAddress (EP1 OUT)
	0x03,		// bmAttributes (Interrupt Transfer)
	0x20, 0x00,	// wMaxPacketSize (TODO need to update this)
	0x05,		// bInterval
	
	/*
	 ****************************************
	 * KEYBOARD INTERFACE
	 ****************************************
	*/
	
	// Second Interface Descriptor (Keyboard Interface)
	0x09,		// bLength
	0x04,		// bDescriptorType (interface)
	0x01,		// bInterfaceNumber (Interface 1)
	0x00,		// bAlternateSetting
	0x02,		// bNumEndpoints
	0x03,		// bInterfaceClass (3 HID)
	0x01,		// bInterfaceSubclass (1 boot)
	0x01,		// bInterfaceProtocol (1 Keyboard)
	0x00,		// bInterface
	
	// HID Descriptor
	0x09,		// bLength
	0x21,		// bDescriptorType (HID)
	0x10, 0x01,	// bcdHID (V1.1)
	0x00,		// bCountryCode
	0x01,		// bNumDescriptors
	0x22,		// bDescriptorType (Still a report descriptor left)
	0x2f, 0x00,	// bDescriptorLength (47 bytes)
	
	// EP1 IN Endpoint Descriptor
	0x07,		// bLength
	0x05,		// bDescriptorType (endpoint)
	0x82,		// bEndpointAddress (EP2 IN)
	0x03,		// bmAttributes (Interrupt Transfer)
	0x08, 0x00,	// wMaxPacketSize
	0x01,		// bInterval (1mS fast)
	
	// EP1 OUT Endpoint Descriptor
	0x07,		// bLength
	0x05,		// BDescriptorType
	0x02,		// bEndpointAddress (EP2 OUT)
	0x03,		// bmAttributes (Interrupt Transfer)
	0x04, 0x00,	// wMaxPacketSize
	0x01		// bInterval (1mS fast)
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
/*
	Make serial something unique so we can try finding it later and replacing it
	we can also put it in a specific location.
	Then you can edit that section of the output hex when programming in production
	You could also read this in from EEPROM
*/
char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  "Mechanical Squid Factory",		// 1: Manufacturer
  "MACRO9PAD",						// 2: Product
  "abc123!",						// 3: Serials, should use chip ID
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
