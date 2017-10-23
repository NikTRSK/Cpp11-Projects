#include "LineShape.h"

LineShape::LineShape(const wxPoint& start) : Shape(start)
{
}

LineShape::~LineShape()
{
}

void LineShape::Draw(wxDC& dc) const
{
	dc.SetPen(GetPen());
	dc.SetBrush(GetBrush());
	
	dc.DrawLine(mStartPoint + mCumulativeOffset + mOffset, mEndPoint + mCumulativeOffset + mOffset);
}
