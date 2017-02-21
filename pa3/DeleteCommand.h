#pragma once
#include "Command.h"
#include "PaintModel.h"
#include "Shape.h"

class DeleteCommand :
	public Command
{
public:
	DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
	~DeleteCommand();
	// Called when the command is completed
	void Finalize(std::shared_ptr<PaintModel> model) override;
	// Used to "undo" the command
	void Undo(std::shared_ptr<PaintModel> model) override;
	// Used to "redo" the command
	void Redo(std::shared_ptr<PaintModel> model) override;
};

