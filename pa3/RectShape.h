#pragma once
#include "Shape.h"
class RectShape : public Shape
{
public:
	RectShape(const wxPoint & start);
	~RectShape();
	void Draw(wxDC& dc) const override;
};

