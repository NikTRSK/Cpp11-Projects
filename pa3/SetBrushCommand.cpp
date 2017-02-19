#include "SetBrushCommand.h"



SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


SetBrushCommand::~SetBrushCommand()
{
}

void SetBrushCommand::Update(const wxPoint& newPoint)
{
}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
}
