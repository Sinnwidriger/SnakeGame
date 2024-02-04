#ifndef SNAKE_GAME_APPLICATIONS_SYSTEM_INFORMATION_APP_H_
#define SNAKE_GAME_APPLICATIONS_SYSTEM_INFORMATION_APP_H_

#include "system_information.h"
#include "system_metric.h"
#include "device_capability.h"
#include "window.h"

namespace applications
{

enum class SysInfoAppContentType
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
	void SetContentType(SysInfoAppContentType content_type);

private:
	SysInfoApp();

	LRESULT HandleCreate(shared::MessageProcParameters mpp);
	LRESULT HandleSize(shared::MessageProcParameters mpp);
	LRESULT HandleScroll(shared::MessageProcParameters mpp, int axis);
	LRESULT HandlePaint(shared::MessageProcParameters mpp);
	LRESULT HandleDestroy(shared::MessageProcParameters mpp);

	virtual LRESULT HandleMessage(shared::MessageProcParameters mpp) override;


	void InitializeCharDimensions();
	void InitializeSystemMetricValues();
	void InitializeDeviceCapabilityValues();
	void DrawSystemInformation(HDC dc, int x, int y, shared::SystemInformation& system_information) const;

	// Fields
	CharDimensions char_dimensions_;

	static constexpr int kFirstColumnCharacters = 22;
	static constexpr int kSecondColumnCharacters = 40;

	std::vector<std::shared_ptr<shared::SystemInformation>> display_information_;

	std::vector<std::shared_ptr<shared::SystemInformation>> system_metrics_ = {
		std::make_shared<shared::SystemMetric>(SM_CXSCREEN, L"SM_CXSCREEN", L"Screen width in pixels:"),
		std::make_shared<shared::SystemMetric>(SM_CYSCREEN, L"SM_CYSCREEN", L"Screen height in pixels:"),
		std::make_shared<shared::SystemMetric>(SM_CXVSCROLL, L"SM_CXVSCROLL", L"Vertical scroll width:"),
		std::make_shared<shared::SystemMetric>(SM_CXHSCROLL, L"SM_CXHSCROLL", L"Horizontal scroll height:"),
		std::make_shared<shared::SystemMetric>(SM_CYCAPTION, L"SM_CYCAPTION", L"Caption bar height:"),
		std::make_shared<shared::SystemMetric>(SM_CXBORDER, L"SM_CXBORDER", L"Window border width:"),
		std::make_shared<shared::SystemMetric>(SM_CYBORDER, L"SM_CYBORDER", L"Window border height:"),
		std::make_shared<shared::SystemMetric>(SM_CXFIXEDFRAME, L"SM_CXFIXEDFRAME", L"Dialog window frame width:"),
		std::make_shared<shared::SystemMetric>(SM_CYFIXEDFRAME, L"SM_CYFIXEDFRAME", L"Dialog window frame height:"),
		std::make_shared<shared::SystemMetric>(SM_CYVTHUMB, L"SM_CYVTHUMB", L"Vertical scroll thumb height:"),
		std::make_shared<shared::SystemMetric>(SM_CXHTHUMB, L"SM_CXHTHUMB", L"Horizontal scroll thumb width:"),
		std::make_shared<shared::SystemMetric>(SM_CXICON, L"SM_CXICON", L"Icon width:"),
		std::make_shared<shared::SystemMetric>(SM_CYICON, L"SM_CYICON", L"Icon height:"),
		std::make_shared<shared::SystemMetric>(SM_CXCURSOR, L"SM_CXCURSOR", L"Cursor width:"),
		std::make_shared<shared::SystemMetric>(SM_CYCURSOR, L"SM_CYCURSOR", L"Cursor height:"),
		std::make_shared<shared::SystemMetric>(SM_CYMENU, L"SM_CYMENU", L"Menu bar height:"),
		std::make_shared<shared::SystemMetric>(SM_CXFULLSCREEN, L"SM_CXFULLSCREEN", L"Full screen client area width:"),
		std::make_shared<shared::SystemMetric>(SM_CYFULLSCREEN, L"SM_CYFULLSCREEN", L"Full screen client area height:"),
		std::make_shared<shared::SystemMetric>(SM_CYKANJIWINDOW, L"SM_CYKANJIWINDOW", L"Kanji window height:"),
		std::make_shared<shared::SystemMetric>(SM_MOUSEPRESENT, L"SM_MOUSEPRESENT", L"Mouse present flag:"),
		std::make_shared<shared::SystemMetric>(SM_DEBUG, L"SM_DEBUG", L"Debug version flag:"),
		std::make_shared<shared::SystemMetric>(SM_SWAPBUTTON, L"SM_SWAPBUTTON", L"Mouse button swapped flag:"),
		std::make_shared<shared::SystemMetric>(SM_CXMIN, L"SM_CXMIN", L"Minimum window width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMIN, L"SM_CYMIN", L"Minimum window height:"),
		std::make_shared<shared::SystemMetric>(SM_CXSIZE, L"SM_CXSIZE", L"Min/Max/Close button width:"),
		std::make_shared<shared::SystemMetric>(SM_CYSIZE, L"SM_CYSIZE", L"Min/Max/Close button height:"),
		std::make_shared<shared::SystemMetric>(SM_CXSIZEFRAME, L"SM_CXSIZEFRAME", L"Window sizing frame width:"),
		std::make_shared<shared::SystemMetric>(SM_CYSIZEFRAME, L"SM_CYSIZEFRAME", L"Window sizing frame height:"),
		std::make_shared<shared::SystemMetric>(SM_CXMINTRACK, L"SM_CXMINTRACK", L"Minimum window tracking width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMINTRACK, L"SM_CYMINTRACK", L"Minimum window tracking height:"),
		std::make_shared<shared::SystemMetric>(SM_CXDOUBLECLK, L"SM_CXDOUBLECLK", L"Double click x tolerance:"),
		std::make_shared<shared::SystemMetric>(SM_CYDOUBLECLK, L"SM_CYDOUBLECLK", L"Double click y tolerance:"),
		std::make_shared<shared::SystemMetric>(SM_CXICONSPACING, L"SM_CXICONSPACING", L"Horizontal icon spacing:"),
		std::make_shared<shared::SystemMetric>(SM_MENUDROPALIGNMENT, L"SM_MENUDROPALIGNMENT", L"Left or right menu drop:"),
		std::make_shared<shared::SystemMetric>(SM_PENWINDOWS, L"SM_PENWINDOWS", L"Pen extensions installed:"),
		std::make_shared<shared::SystemMetric>(SM_DBCSENABLED, L"SM_DBCSENABLED", L"Double-Byte Char Set enabled:"),
		std::make_shared<shared::SystemMetric>(SM_CMOUSEBUTTONS, L"SM_CMOUSEBUTTONS", L"Number of mouse buttons:"),
		std::make_shared<shared::SystemMetric>(SM_SECURE, L"SM_SECURE", L"Security present flag:"),
		std::make_shared<shared::SystemMetric>(SM_CXEDGE, L"SM_CXEDGE", L"3-D border width:"),
		std::make_shared<shared::SystemMetric>(SM_CYEDGE, L"SM_CYEDGE", L"3-D border height:"),
		std::make_shared<shared::SystemMetric>(SM_CXMINSPACING, L"SM_CXMINSPACING", L"Minimized window spacing width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMINSPACING, L"SM_CYMINSPACING", L"Minimized window spacing height:"),
		std::make_shared<shared::SystemMetric>(SM_CXSMICON, L"SM_CXSMICON", L"Small icon width:"),
		std::make_shared<shared::SystemMetric>(SM_CYSMICON, L"SM_CYSMICON", L"Small icon height:"),
		std::make_shared<shared::SystemMetric>(SM_CYSMCAPTION, L"SM_CYSMCAPTION", L"Small caption height:"),
		std::make_shared<shared::SystemMetric>(SM_CXSMSIZE, L"SM_CXSMSIZE", L"Small caption button width:"),
		std::make_shared<shared::SystemMetric>(SM_CYSMSIZE, L"SM_CYSMSIZE", L"Small caption button height:"),
		std::make_shared<shared::SystemMetric>(SM_CXMENUSIZE, L"SM_CXMENUSIZE", L"Menu bar button width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMENUSIZE, L"SM_CYMENUSIZE", L"Menu bar button height:"),
		std::make_shared<shared::SystemMetric>(SM_ARRANGE, L"SM_ARRANGE", L"How minimized windows arranged:"),
		std::make_shared<shared::SystemMetric>(SM_CXMINIMIZED, L"SM_CXMINIMIZED", L"Minimized window width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMINIMIZED, L"SM_CYMINIMIZED", L"Minimized window height:"),
		std::make_shared<shared::SystemMetric>(SM_CXMAXTRACK, L"SM_CXMAXTRACK", L"Maximum draggable width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMAXTRACK, L"SM_CYMAXTRACK", L"Maximum draggable height:"),
		std::make_shared<shared::SystemMetric>(SM_CXMAXIMIZED, L"SM_CXMAXIMIZED", L"Width of maximized window:"),
		std::make_shared<shared::SystemMetric>(SM_CYMAXIMIZED, L"SM_CYMAXIMIZED", L"Height of maximized window:"),
		std::make_shared<shared::SystemMetric>(SM_NETWORK, L"SM_NETWORK", L"Network present flag:"),
		std::make_shared<shared::SystemMetric>(SM_CLEANBOOT, L"SM_CLEANBOOT", L"How system was booted:"),
		std::make_shared<shared::SystemMetric>(SM_CXDRAG, L"SM_CXDRAG", L"Avoid drag x tolerance:"),
		std::make_shared<shared::SystemMetric>(SM_CYDRAG, L"SM_CYDRAG", L"Avoid drag y tolerance:"),
		std::make_shared<shared::SystemMetric>(SM_SHOWSOUNDS, L"SM_SHOWSOUNDS", L"Present sounds visually:"),
		std::make_shared<shared::SystemMetric>(SM_CXMENUCHECK, L"SM_CXMENUCHECK", L"Menu check-mark width:"),
		std::make_shared<shared::SystemMetric>(SM_CYMENUCHECK, L"SM_CYMENUCHECK", L"Menu check-mark height:"),
		std::make_shared<shared::SystemMetric>(SM_SLOWMACHINE, L"SM_SLOWMACHINE", L"Slow processor flag:"),
		std::make_shared<shared::SystemMetric>(SM_MIDEASTENABLED, L"SM_MIDEASTENABLED", L"Hebrew and Arabic enabled flag:"),
		std::make_shared<shared::SystemMetric>(SM_MOUSEWHEELPRESENT, L"SM_MOUSEWHEELPRESENT", L"Mouse wheel present flag:"),
		std::make_shared<shared::SystemMetric>(SM_XVIRTUALSCREEN, L"SM_XVIRTUALSCREEN", L"Virtual screen x origin:"),
		std::make_shared<shared::SystemMetric>(SM_YVIRTUALSCREEN, L"SM_YVIRTUALSCREEN", L"Virtual screen y origin:"),
		std::make_shared<shared::SystemMetric>(SM_CXVIRTUALSCREEN, L"SM_CXVIRTUALSCREEN", L"Virtual screen width:"),
		std::make_shared<shared::SystemMetric>(SM_CYVIRTUALSCREEN, L"SM_CYVIRTUALSCREEN", L"Virtual screen height:"),
		std::make_shared<shared::SystemMetric>(SM_CMONITORS, L"SM_CMONITORS", L"Number of monitors:"),
		std::make_shared<shared::SystemMetric>(SM_SAMEDISPLAYFORMAT, L"SM_SAMEDISPLAYFORMAT", L"Same color format flag:")
	};

	std::vector<std::shared_ptr<shared::SystemInformation>> device_capabilities_ = {
		std::make_shared<shared::DeviceCapability>(HORZSIZE, L"HORZSIZE", L"Width in millimeters:"),
		std::make_shared<shared::DeviceCapability>(VERTSIZE, L"VERTSIZE", L"Height in millimeters:"),
		std::make_shared<shared::DeviceCapability>(HORZRES, L"HORZRES", L"Width in pixels:"),
		std::make_shared<shared::DeviceCapability>(VERTRES, L"VERTRES", L"Height in raster lines:"),
		std::make_shared<shared::DeviceCapability>(BITSPIXEL, L"BITSPIXEL", L"Color bits per pixel:"),
		std::make_shared<shared::DeviceCapability>(PLANES, L"PLANES", L"Number of color planes:"),
		std::make_shared<shared::DeviceCapability>(NUMBRUSHES, L"NUMBRUSHES", L"Number of device brushes:"),
		std::make_shared<shared::DeviceCapability>(NUMPENS, L"NUMPENS", L"Number of device pens:"),
		std::make_shared<shared::DeviceCapability>(NUMMARKERS, L"NUMMARKERS", L"Number of device markers:"),
		std::make_shared<shared::DeviceCapability>(NUMFONTS, L"NUMFONTS", L"Number of device fonts:"),
		std::make_shared<shared::DeviceCapability>(NUMCOLORS, L"NUMCOLORS", L"Number of device colors:"),
		std::make_shared<shared::DeviceCapability>(PDEVICESIZE, L"PDEVICESIZE", L"Size of device structure:"),
		std::make_shared<shared::DeviceCapability>(ASPECTX, L"ASPECTX", L"Relative width of pixel:"),
		std::make_shared<shared::DeviceCapability>(ASPECTY, L"ASPECTY", L"Relative height of pixel:"),
		std::make_shared<shared::DeviceCapability>(ASPECTXY, L"ASPECTXY", L"Relative diagonal of pixel:"),
		std::make_shared<shared::DeviceCapability>(LOGPIXELSX, L"LOGPIXELSX", L"Horizontal dots per inch:"),
		std::make_shared<shared::DeviceCapability>(LOGPIXELSY, L"LOGPIXELSY", L"Vertical dots per inch:"),
		std::make_shared<shared::DeviceCapability>(SIZEPALETTE, L"SIZEPALETTE", L"Number of palette entries:"),
		std::make_shared<shared::DeviceCapability>(NUMRESERVED, L"NUMRESERVED", L"Reserved palette entries:"),
		std::make_shared<shared::DeviceCapability>(COLORRES, L"COLORRES", L"Actual color resolution:")
	};
};

}

#endif