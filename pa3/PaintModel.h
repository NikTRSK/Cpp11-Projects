#pragma once
#include <memory>
#include <vector>
#include <stack>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();

	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);

	//USER DEFINED//
	// Returns true if there’s currently an active command
	bool HasActiveCommand();
	// Creates a command of a specific type
	void CreateCommand(CommandType commandType, const wxPoint& start);
	// Updates the current Command
	void UpdateCommand(const wxPoint& newPoint);
	void FinalizeCommand();
	// Adds the current command to the undo stack
	void AddCurrentCommandToUndoStack();
	// Returns false if undo stack is empty
	bool CanUndo();
	// Returns false if redo stack is empty
	bool CanRedo();
	// Returns the removed command  and push is to the Redo stack
	// Assumes that the stack is not empty
	void Undo();
	// Returns the removed command  and push is to the Redo stack
	// Assumes that the stack is not empty
	void Redo();
	std::shared_ptr<Command> & GetTopInUndo();
	std::shared_ptr<Command> & GetTopInRedo();

	// Shape Stacks
	void UndoShape();
	void RedoShape();
	std::stack<std::shared_ptr<Shape>> & GetTopInUndoShape();
	std::stack<std::shared_ptr<Shape>> & GetTopInRedoShape();
	void AddSelectedShapeToUndoStack();

	std::shared_ptr<Command> & GetCurrentCommand();
	// Clear stack
	void ClearRedo();
	void ClearUndo();
	std::vector<std::shared_ptr<Shape>> & GetShapes();

	int GetPenWidth();
	const wxPen & GetPen();
	const wxBrush & GetBrush();

	void SetPenWidth(const int & size);
	void SetPenColor(const wxColour& color);
	void SetBrushColor(const wxColour& color);

	std::shared_ptr<Shape> & GetSelectedShape();
	bool SelectShape(wxPoint point);

	void Export(wxString fname, wxSize imageSize);
	void Import(const wxString & fname);

private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
	std::shared_ptr<Command> mCurrentActiveCommand;

	// Undo/Redo Stacks
	std::stack<std::shared_ptr<Command>> mUndo;
	std::stack<std::shared_ptr<Command>> mRedo;
	// Shape Stacks
	std::stack<std::shared_ptr<Shape>> mUndoShape;
	std::stack<std::shared_ptr<Shape>> mRedoShape;
	// Pen/Brush Stacks
	std::stack<wxPen> mUndoPen;
	std::stack<wxPen> mRedoPen;
	std::stack<wxBrush> mUndoBrush;
	std::stack<wxBrush> mRedoBrush;

	// Pen and Brush
	wxPen mPen;
	wxBrush mBrush;

	// The shape currently selected by the selection tool
	std::shared_ptr<Shape> mSelectedShape;

	// Store the imported image
	wxBitmap mImage;
};
