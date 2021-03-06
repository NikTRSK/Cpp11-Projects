#pragma once
#include "Command.h"
#include "PaintModel.h"
class SetPenCommand : public Command
{
public:
	SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
	~SetPenCommand();
	// Called when the command is still updating (such as in the process of drawing)
	virtual void Update(const wxPoint& newPoint) override;
	// Called when the command is completed
	virtual void Finalize(std::shared_ptr<PaintModel> model) override;
	// Used to "undo" the command
	virtual void Undo(std::shared_ptr<PaintModel> model) override;
	// Used to "redo" the command
	virtual void Redo(std::shared_ptr<PaintModel> model) override;

private:
	// For Undo/Redo of Pens
	wxPen mCurrPen;
	wxPen mOldPen;
};

