#include "RectShape.h"



RectShape::RectShape(const wxPoint& start) : Shape(start)
{
}


RectShape::~RectShape()
{
}

void RectShape::Draw(wxDC & dc) const
{
	dc.DrawRectangle(wxRect(this->mTopLeft, this->mBotRight));
}

/*
 *
 * A wxRect can be constructed by two points – the top left and bottom right ones,
 * which conveniently we have as members of shape.
 */

/*
 *void 	DrawRectangle (const wxRect &rect)
 	This is an overloaded member function, provided for convenience.
It differs from the above function only in what argument(s) it accepts. More...
 *
 *
 */