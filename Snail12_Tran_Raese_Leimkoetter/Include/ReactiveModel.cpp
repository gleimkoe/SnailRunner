#include "ReactiveModel.h"
#include <iostream>

// --Default implementation is empty.
void ReactiveModel::onCounterChanged(Bitfield /*bfield*/) {}
void ReactiveModel::onCounterConfChanged(Bitfield /*bfield*/) {}
void ReactiveModel::onInputChanged(Bitfield /*bfield*/) {}
void ReactiveModel::onMotorStopped(Bitfield /*bfield*/) {}
void ReactiveModel::onMotorAborted(Bitfield /*bfield*/) {}
void ReactiveModel::onTimerChanged(Bitfield /*bfield*/) {}

void ReactiveModel::onStart() {}
void ReactiveModel::onStop() {}
void ReactiveModel::failure() {}
void ReactiveModel::recover() {}	

bool ReactiveModel::isFinished() { return _isFinished; }
void ReactiveModel::onError(const char* err) {
	std::cerr << "\n*** REACTIVE MODEL ERROR: " << err << ". ***\n" << std::endl;
}

TxControllerSupervision* ReactiveModel::controller() const { return dynamic_cast<TxControllerSupervision*>(m_Controller); }

ReactiveModel::Bitfield ReactiveModel::counterMask() const { return _counterMask; }
ReactiveModel::Bitfield ReactiveModel::counterConfMask() const { return _counterConfMask; }
ReactiveModel::Bitfield ReactiveModel::inputMask() const { return _inputMask; }
ReactiveModel::Bitfield ReactiveModel::motorMask() const { return _motorMask; }
ReactiveModel::Bitfield ReactiveModel::abortMask() const { return _abortMask; }

void ReactiveModel::setIsFinished(bool val) {
	_isFinished=val;
}

ReactiveModel::ReactiveModel():_isFinished(false),_counterMask(0xff),_counterConfMask(0xff),_inputMask(0xff),_motorMask(0xff),
	_abortMask(0xff) ,_current(-1) {}

/* --Timer-Funktionen. */

TimerModel& ReactiveModel::getTimer(TimerIdentifier tid) {
	return _timer[tid];
}


TimingInformation ReactiveModel::current() const { 
	return _current;
}

void ReactiveModel::current(TimingInformation time) {
	_current=time;
}
	
void ReactiveModel::resetTimer(TimerIdentifier tid, TimingInformation period, TimingInformation continuation) {
	_timer[tid].isActive=false;
	_timer[tid].period=period;
	_timer[tid].final=continuation;
}

bool ReactiveModel::startTimer(TimerIdentifier tid, TimingInformation offset) {
	bool retvalue=true;
	if (_timer[tid].isActive==false && _current!=-1) {
		_timer[tid].alarm=_current+offset+_timer[tid].period;
		if (_timer[tid].final!=TIMING_FOREVER)
			_timer[tid].final=_current+_timer[tid].final;
		_timer[tid].isActive=true;
	}
	else
		retvalue=false;
	return retvalue;
}

bool ReactiveModel::stopTimer(TimerIdentifier tid) {
	bool retvalue=true;
	if (_timer[tid].isActive)
		_timer[tid].isActive=false;
	else
		retvalue=false;
	return retvalue;
}
