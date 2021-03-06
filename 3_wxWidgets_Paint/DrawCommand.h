#pragma once
#include "Command.h"
#include "PaintModel.h"
#include "Shape.h"

class DrawCommand :	public Command
{
public:
	DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
	~DrawCommand();
	void Update(const wxPoint& newPoint) override;
	// Called when the command is completed
	void Finalize(std::shared_ptr<PaintModel> model) override;
	// Used to "undo" the command
	void Undo(std::shared_ptr<PaintModel> model) override;
	// Used to "redo" the command
	void Redo(std::shared_ptr<PaintModel> model) override;
};

