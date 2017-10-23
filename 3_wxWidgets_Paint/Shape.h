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

	// Pen Functions specific for the shape
	wxPen GetPen() const;
	int GetPenWidth() const;
	void SetPenWidth(const int & size);
	void SetPenColor(const wxColour& color);
	// Brush Functioins specific for the shape
	wxBrush GetBrush() const;
	void SetBrushColor(const wxColour& color);
	// Draws a selection around the selected shape
	void DrawSelection(wxDC& dc);
	// Offsets used for the Move Function
	const wxPoint & GetOffset();
	const wxPoint & GetCumulativeOffset();
	void SetOffset(const wxPoint & newOffset);
	void SetCumulativeOffset(const wxPoint & newOffset);
	void AddToCumulativeOffset(const wxPoint & toAdd);

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
	wxPoint mCumulativeOffset;
	wxPoint mOffset;
	
};
