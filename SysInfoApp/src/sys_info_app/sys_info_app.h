#pragma once
#pragma once

#include "sys_info_types/system_information.h"
#include "sys_info_types/system_metric.h"
#include "sys_info_types/device_capability.h"

enum class SysInfoAppContent
{
	kSystemMetrics = 0,
	kDeviceCapabilities = 1
};

struct CharDimensions
{
	unsigned int lower_case_width;
	unsigned int upper_case_width;
	unsigned int height;
};

class SysInfoApp : public shared::Window
{
	friend class Window;

public:
	void SetContentType(SysInfoAppContent content_type);

private:
	SysInfoApp();

	LRESULT HandleCreate(shared::MessageProcParameters mpp);
	LRESULT HandleSize(shared::MessageProcParameters mpp);
	LRESULT HandleVertScroll(shared::MessageProcParameters mpp);
	LRESULT HandleHorzScroll(shared::MessageProcParameters mpp);
	LRESULT HandleScroll(shared::MessageProcParameters mpp, int axis);
	LRESULT HandlePaint(shared::MessageProcParameters mpp);
	LRESULT HandleDestroy(shared::MessageProcParameters mpp);


	void InitializeCharDimensions();
	void InitializeSystemMetricValues();
	void InitializeDeviceCapabilityValues();
	void DrawSystemInformation(HDC dc, int x, int y, sys_info_types::SystemInformation& system_information) const;

	// Fields
	CharDimensions char_dimensions_;

	static constexpr int kFirstColumnCharacters = 22;
	static constexpr int kSecondColumnCharacters = 40;

	std::unordered_map<SysInfoAppContent, std::vector<std::shared_ptr<sys_info_types::SystemInformation>>> content_type_map_;

	std::vector<std::shared_ptr<sys_info_types::SystemInformation>> display_information_;

	std::vector<std::shared_ptr<sys_info_types::SystemInformation>> system_metrics_ = {
		std::make_shared<sys_info_types::SystemMetric>(SM_CXSCREEN, L"SM_CXSCREEN", L"Screen width in pixels:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSCREEN, L"SM_CYSCREEN", L"Screen height in pixels:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXVSCROLL, L"SM_CXVSCROLL", L"Vertical scroll width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXHSCROLL, L"SM_CXHSCROLL", L"Horizontal scroll height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYCAPTION, L"SM_CYCAPTION", L"Caption bar height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXBORDER, L"SM_CXBORDER", L"Window border width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYBORDER, L"SM_CYBORDER", L"Window border height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXFIXEDFRAME, L"SM_CXFIXEDFRAME", L"Dialog window frame width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYFIXEDFRAME, L"SM_CYFIXEDFRAME", L"Dialog window frame height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYVTHUMB, L"SM_CYVTHUMB", L"Vertical scroll thumb height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXHTHUMB, L"SM_CXHTHUMB", L"Horizontal scroll thumb width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXICON, L"SM_CXICON", L"Icon width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYICON, L"SM_CYICON", L"Icon height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXCURSOR, L"SM_CXCURSOR", L"Cursor width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYCURSOR, L"SM_CYCURSOR", L"Cursor height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMENU, L"SM_CYMENU", L"Menu bar height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXFULLSCREEN, L"SM_CXFULLSCREEN", L"Full screen client area width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYFULLSCREEN, L"SM_CYFULLSCREEN", L"Full screen client area height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYKANJIWINDOW, L"SM_CYKANJIWINDOW", L"Kanji window height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_MOUSEPRESENT, L"SM_MOUSEPRESENT", L"Mouse present flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_DEBUG, L"SM_DEBUG", L"Debug version flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_SWAPBUTTON, L"SM_SWAPBUTTON", L"Mouse button swapped flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMIN, L"SM_CXMIN", L"Minimum window width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMIN, L"SM_CYMIN", L"Minimum window height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXSIZE, L"SM_CXSIZE", L"Min/Max/Close button width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSIZE, L"SM_CYSIZE", L"Min/Max/Close button height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXSIZEFRAME, L"SM_CXSIZEFRAME", L"Window sizing frame width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSIZEFRAME, L"SM_CYSIZEFRAME", L"Window sizing frame height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMINTRACK, L"SM_CXMINTRACK", L"Minimum window tracking width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMINTRACK, L"SM_CYMINTRACK", L"Minimum window tracking height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXDOUBLECLK, L"SM_CXDOUBLECLK", L"Double click x tolerance:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYDOUBLECLK, L"SM_CYDOUBLECLK", L"Double click y tolerance:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXICONSPACING, L"SM_CXICONSPACING", L"Horizontal icon spacing:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_MENUDROPALIGNMENT, L"SM_MENUDROPALIGNMENT", L"Left or right menu drop:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_PENWINDOWS, L"SM_PENWINDOWS", L"Pen extensions installed:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_DBCSENABLED, L"SM_DBCSENABLED", L"Double-Byte Char Set enabled:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CMOUSEBUTTONS, L"SM_CMOUSEBUTTONS", L"Number of mouse buttons:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_SECURE, L"SM_SECURE", L"Security present flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXEDGE, L"SM_CXEDGE", L"3-D border width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYEDGE, L"SM_CYEDGE", L"3-D border height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMINSPACING, L"SM_CXMINSPACING", L"Minimized window spacing width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMINSPACING, L"SM_CYMINSPACING", L"Minimized window spacing height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXSMICON, L"SM_CXSMICON", L"Small icon width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSMICON, L"SM_CYSMICON", L"Small icon height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSMCAPTION, L"SM_CYSMCAPTION", L"Small caption height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXSMSIZE, L"SM_CXSMSIZE", L"Small caption button width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYSMSIZE, L"SM_CYSMSIZE", L"Small caption button height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMENUSIZE, L"SM_CXMENUSIZE", L"Menu bar button width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMENUSIZE, L"SM_CYMENUSIZE", L"Menu bar button height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_ARRANGE, L"SM_ARRANGE", L"How minimized windows arranged:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMINIMIZED, L"SM_CXMINIMIZED", L"Minimized window width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMINIMIZED, L"SM_CYMINIMIZED", L"Minimized window height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMAXTRACK, L"SM_CXMAXTRACK", L"Maximum draggable width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMAXTRACK, L"SM_CYMAXTRACK", L"Maximum draggable height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMAXIMIZED, L"SM_CXMAXIMIZED", L"Width of maximized window:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMAXIMIZED, L"SM_CYMAXIMIZED", L"Height of maximized window:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_NETWORK, L"SM_NETWORK", L"Network present flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CLEANBOOT, L"SM_CLEANBOOT", L"How system was booted:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXDRAG, L"SM_CXDRAG", L"Avoid drag x tolerance:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYDRAG, L"SM_CYDRAG", L"Avoid drag y tolerance:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_SHOWSOUNDS, L"SM_SHOWSOUNDS", L"Present sounds visually:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXMENUCHECK, L"SM_CXMENUCHECK", L"Menu check-mark width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYMENUCHECK, L"SM_CYMENUCHECK", L"Menu check-mark height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_SLOWMACHINE, L"SM_SLOWMACHINE", L"Slow processor flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_MIDEASTENABLED, L"SM_MIDEASTENABLED", L"Hebrew and Arabic enabled flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_MOUSEWHEELPRESENT, L"SM_MOUSEWHEELPRESENT", L"Mouse wheel present flag:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_XVIRTUALSCREEN, L"SM_XVIRTUALSCREEN", L"Virtual screen x origin:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_YVIRTUALSCREEN, L"SM_YVIRTUALSCREEN", L"Virtual screen y origin:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CXVIRTUALSCREEN, L"SM_CXVIRTUALSCREEN", L"Virtual screen width:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CYVIRTUALSCREEN, L"SM_CYVIRTUALSCREEN", L"Virtual screen height:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_CMONITORS, L"SM_CMONITORS", L"Number of monitors:"),
		std::make_shared<sys_info_types::SystemMetric>(SM_SAMEDISPLAYFORMAT, L"SM_SAMEDISPLAYFORMAT", L"Same color format flag:")
	};

	std::vector<std::shared_ptr<sys_info_types::SystemInformation>> device_capabilities_ = {
		std::make_shared<sys_info_types::DeviceCapability>(HORZSIZE, L"HORZSIZE", L"Width in millimeters:"),
		std::make_shared<sys_info_types::DeviceCapability>(VERTSIZE, L"VERTSIZE", L"Height in millimeters:"),
		std::make_shared<sys_info_types::DeviceCapability>(HORZRES, L"HORZRES", L"Width in pixels:"),
		std::make_shared<sys_info_types::DeviceCapability>(VERTRES, L"VERTRES", L"Height in raster lines:"),
		std::make_shared<sys_info_types::DeviceCapability>(BITSPIXEL, L"BITSPIXEL", L"Color bits per pixel:"),
		std::make_shared<sys_info_types::DeviceCapability>(PLANES, L"PLANES", L"Number of color planes:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMBRUSHES, L"NUMBRUSHES", L"Number of device brushes:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMPENS, L"NUMPENS", L"Number of device pens:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMMARKERS, L"NUMMARKERS", L"Number of device markers:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMFONTS, L"NUMFONTS", L"Number of device fonts:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMCOLORS, L"NUMCOLORS", L"Number of device colors:"),
		std::make_shared<sys_info_types::DeviceCapability>(PDEVICESIZE, L"PDEVICESIZE", L"Size of device structure:"),
		std::make_shared<sys_info_types::DeviceCapability>(ASPECTX, L"ASPECTX", L"Relative width of pixel:"),
		std::make_shared<sys_info_types::DeviceCapability>(ASPECTY, L"ASPECTY", L"Relative height of pixel:"),
		std::make_shared<sys_info_types::DeviceCapability>(ASPECTXY, L"ASPECTXY", L"Relative diagonal of pixel:"),
		std::make_shared<sys_info_types::DeviceCapability>(LOGPIXELSX, L"LOGPIXELSX", L"Horizontal dots per inch:"),
		std::make_shared<sys_info_types::DeviceCapability>(LOGPIXELSY, L"LOGPIXELSY", L"Vertical dots per inch:"),
		std::make_shared<sys_info_types::DeviceCapability>(SIZEPALETTE, L"SIZEPALETTE", L"Number of palette entries:"),
		std::make_shared<sys_info_types::DeviceCapability>(NUMRESERVED, L"NUMRESERVED", L"Reserved palette entries:"),
		std::make_shared<sys_info_types::DeviceCapability>(COLORRES, L"COLORRES", L"Actual color resolution:")
	};
};