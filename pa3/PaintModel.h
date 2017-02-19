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
	const std::shared_ptr<Command> & GetTopInUndo();
	const std::shared_ptr<Command> & GetTopInRedo();

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

private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
	std::shared_ptr<Command> mCurrentActiveCommand;

	// Undo/Redo Stacks
	std::stack<std::shared_ptr<Command>> mUndo;
	std::stack<std::shared_ptr<Command>> mRedo;

	// Pen and Brush
	wxPen mPen;
	wxBrush mBrush;
};
