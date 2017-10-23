#include "EllipseShape.h"



EllipseShape::EllipseShape(const wxPoint& start) : Shape(start)
{
}

EllipseShape::~EllipseShape()
{
}

void EllipseShape::Draw(wxDC & dc) const
{
	dc.SetPen(GetPen());
	dc.SetBrush(GetBrush());

	// Get the Ellipse bounds
	wxPoint x;
	wxPoint y;
	GetBounds(x, y);

	dc.DrawEllipse(wxRect(x, y));
}
