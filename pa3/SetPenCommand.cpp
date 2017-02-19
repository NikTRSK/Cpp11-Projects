#include "SetPenCommand.h"



SetPenCommand::SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


SetPenCommand::~SetPenCommand()
{
}

void SetPenCommand::Update(const wxPoint& newPoint)
{
}

void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
}
