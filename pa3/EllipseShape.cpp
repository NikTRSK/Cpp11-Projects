#include "EllipseShape.h"



EllipseShape::EllipseShape(const wxPoint& start) : Shape(start)
{
}

EllipseShape::~EllipseShape()
{
}

void EllipseShape::Draw(wxDC & dc) const
{
	dc.DrawEllipse(wxRect(this->mTopLeft, this->mBotRight));
}
