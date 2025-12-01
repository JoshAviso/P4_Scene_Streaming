#pragma once

class IWorkerTask {
public:
	virtual void DoWorkerTask(int id) = 0;
};


