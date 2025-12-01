#pragma once

class IThreadFinishedCallback {
public:
	virtual void OnThreadFinished(int id) = 0;
};