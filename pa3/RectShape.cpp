#include "RectShape.h"



RectShape::RectShape(const wxPoint& start) : Shape(start)
{
}

RectShape::~RectShape()
{
}

void RectShape::Draw(wxDC & dc) const
{
	dc.SetPen(GetPen());
	dc.SetBrush(GetBrush());

	// Get the rectangle bounds
	wxPoint x;
	wxPoint y;
	GetBounds(x, y);

	dc.DrawRectangle(wxRect(x, y));
}