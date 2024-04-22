#pragma once


#ifdef _WIN32
#define VGUI2_LIB "vgui2.dll"
#else
#define VGUI2_LIB "vgui2.so"
#endif // _WIN32


namespace vgui2
{
	enum DataType_t
	{
		DATATYPE_VOID,
		DATATYPE_CONSTCHARPTR,
		DATATYPE_INT,
		DATATYPE_FLOAT,
		DATATYPE_PTR ,
		DATATYPE_BOOL,
		DATATYPE_KEYVALUES ,
		DATATYPE_CONSTWCHARPTR,
		DATATYPE_UINT64,
	};

	// handle to an internal vgui panel
	// this is the only handle to a panel that is valid across dll boundaries
	typedef unsigned int VPANEL;
	// handles to vgui objects
	// NULL values signify an invalid value
	typedef unsigned long HScheme;
	// Both -1 and 0 are used for invalid textures. Be careful.
	typedef unsigned long HTexture;
	typedef unsigned long HCursor;
	typedef unsigned long HPanel;
	//const HPanel INVALID_PANEL = 0xffffffff;
	typedef unsigned long HFont;
	//const HFont INVALID_FONT = 0; // the value of an invalid font handle
	
	class Panel;

	class PHandle
	{
	public:
		Panel *Get();
		Panel *Set(Panel *pent);
	private:
		HPanel m_iPanelID;
	};

	class VPanelHandle
	{
	public:
		VPANEL Get();
		VPANEL Set(VPANEL pent);

	private:
		HPanel m_iPanelID;
	};

	struct VGuiVertex
	{
		int x;
		int y;
		float u;
		float v;
	};
}