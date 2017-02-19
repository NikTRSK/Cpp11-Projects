#include "PencilShape.h"


/* TODO: Right now draws lines only. */
PencilShape::PencilShape(const wxPoint & start) : Shape(start)
{
	mPoints.push_back(start);
}


PencilShape::~PencilShape()
{
}

void PencilShape::Draw(wxDC& dc) const
{
	// DrawLines accepts 2 or more points so we need
	// to handle the case of just having a dot.
	if(mPoints.size() == 1)
	{
		dc.DrawPoint(*mPoints.begin());
	}
	else if (mPoints.size() > 1)
	{
		dc.DrawLines(mPoints.size(), &mPoints.front());
	}
}

void PencilShape::Update(const wxPoint& newPoint)
{
	Shape::Update(newPoint);
	mPoints.push_back(newPoint);
}

void PencilShape::Finalize()
{
	// Initialize the points iterator
	auto point = mPoints.begin();

	// Get the first points components
	int top = point->y,
		bottom = point->y,
		left = point->x,
		right = point->x;

	/* left <=
	 * right =>
	 *
	 */
	for (point; point != mPoints.end(); ++point)
	{
		if (point->y < top)
		{
			top = point->y;
		}
		if (point->y > bottom)
		{
			bottom = point->y;
		}
		if (point->y < left)
		{
			left = point->y;
		}
		if (point->x > right)
		{
			right = point->x;
		}

		mTopLeft.x = left;
		mTopLeft.y = top;
		mBotRight.x = right;
		mBotRight.y = bottom;
	}

}
