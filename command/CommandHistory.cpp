#include "CommandHistory.h"

void CommandHistory::PushCommand(std::unique_ptr<Command> cmd)
{
    if (!cmd) return;
    cmd->Execute();
    mUndoStack.push_back(std::move(cmd));
    mRedoStack.clear(); // Xóa sạch Redo stack khi có lệnh mới
}

bool CommandHistory::Undo()
{
    if (mUndoStack.empty()) return false;

    std::unique_ptr<Command> cmd = std::move(mUndoStack.back());
    mUndoStack.pop_back();

    cmd->Undo();
    mRedoStack.push_back(std::move(cmd));
    return true;
}

bool CommandHistory::Redo()
{
    if (mRedoStack.empty()) return false;

    std::unique_ptr<Command> cmd = std::move(mRedoStack.back());
    mRedoStack.pop_back();

    cmd->Execute();
    mUndoStack.push_back(std::move(cmd));
    return true;
}

void CommandHistory::Clear()
{
    mUndoStack.clear();
    mRedoStack.clear();
}

bool CommandHistory::CanUndo() const
{
    return !mUndoStack.empty();
}

bool CommandHistory::CanRedo() const
{
    return !mRedoStack.empty();
}
