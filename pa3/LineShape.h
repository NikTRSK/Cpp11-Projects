#pragma once
#include "Shape.h"
class LineShape :
	public Shape
{
public:
	LineShape(const wxPoint & start);
	~LineShape();
	void Draw(wxDC& dc) const override;
};

