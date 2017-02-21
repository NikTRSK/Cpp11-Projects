#pragma once
#include "Command.h"
#include "PaintModel.h"
class SetBrushCommand :
	public Command
{
public:
	SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
	~SetBrushCommand();
	// Called when the command is still updating (such as in the process of drawing)
	virtual void Update(const wxPoint& newPoint) override;
	// Called when the command is completed
	virtual void Finalize(std::shared_ptr<PaintModel> model) override;
	// Used to "undo" the command
	virtual void Undo(std::shared_ptr<PaintModel> model) override;
	// Used to "redo" the command
	virtual void Redo(std::shared_ptr<PaintModel> model) override;

	// do we need shape var here
	std::stack <wxBrush> mUndoBrush;
	std::stack <wxBrush> mRedoBrush;
};

