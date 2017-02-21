#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
	virtual ~Shape() { }

	int GetPenWidth();
	wxPen GetPen() const;
	wxBrush GetBrush() const;

	void SetPenWidth(const int & size);
	void SetPenColor(const wxColour& color);
	void SetBrushColor(const wxColour& color);
	void DrawSelection(wxDC& dc);
	const wxPoint & GetOffset();
	void UpdateOffset(const wxPoint & newOffset);
protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;

	// Pen and Brush
	wxPen mPen;
	wxBrush mBrush;

	// Offset required for MoveCommand
	wxPoint mOffset;
};
