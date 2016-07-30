#ifndef __Z2pho_h__
#define __Z2pho_h__

typedef unsigned char		byte;

#include <Canvas\Canvas.h>
#include <Channel\Channel.h>
#include <Selare\Selare.h>

#include <Space\ColorSpace.h>

#include <Shape\Shape.h>
#include <Shape\LineShape.h>
#include <Shape\TriangleShape.h>
#include <Shape\RectangleShape.h>
#include <Shape\CircleShape.h>
#include <Shape\EllipseShape.h>
#include <Shape\PolygonShape.h>

#include <Curve\Curve.h>
#include <Curve\BezierBase.h>
#include <Curve\Bezier2D.h>
#include <Curve\Bezier3D.h>
#include <Curve\BezierND.h>
#include <Curve\BezierCurve.h>

#include <Fill\Fill.h>
#include <Fill\SolidFill.h>
#include <Fill\HatchFill.h>
#include <Fill\GradientFill.h>

#include <Brush\Brush.h>
#include <Brush\BasicBrush.h>
#include <Brush\ImageBrush.h>

#include <Fractal\CSystem\MandelCDS.h>
#include <Fractal\CSystem\JuliaCDS.h>
#include <Fractal\CSystem\NewtonCDS.h>
#include <Fractal\CSystem\NovaCDS.h>

#include <Blend\ChanBlend.h>
#include <Blend\ColorBlend.h>
#include <Blend\ImageBlend.h>

#include <Filter\Filter.h>
#include <Filter\ReliefFilter.h>
#include <Filter\NeonFilter.h>
#include <Filter\FrostedFilter.h>
#include <Filter\SketchFilter.h>

#include <Layer\Layer.h>
#include <Layer\LayerManage.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "Z2phoD.lib")
	#else
		#pragma comment(lib, "Z2pho.lib")
	#endif
#endif

class CZ2pho {
  public:
	SINGLETON(CZ2pho);
	~CZ2pho();

  private:
	CZ2pho();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	CWindow			*GetWindow() const;
	void			 SetColor(COLORREF color);
	COLORREF		 GetColor() const;
	void			 SetBkColor(COLORREF color);
	COLORREF		 GetBkColor() const;

  private:
	CWindow			*window;
	CRect			 ClientRect;

  private:
	COLORREF		 m_color;
	COLORREF		 m_bkcolor;
};

inline void CZ2pho::SetWindow(CWindow *_window)
{
	window = _window;
	window->GetClientRect(&ClientRect);
}

inline CWindow *CZ2pho::GetWindow() const
{
	return window;
}

inline void CZ2pho::SetColor(COLORREF color)
{
	m_color = color;
}

inline COLORREF CZ2pho::GetColor() const
{
	return m_color;
}

inline void CZ2pho::SetBkColor(COLORREF color)
{
	m_bkcolor = color;
}

inline COLORREF CZ2pho::GetBkColor() const
{
	return m_bkcolor;
}

#define g_pZ2pho		CZ2pho::GetInstancePtr()

#endif
