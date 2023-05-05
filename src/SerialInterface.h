#ifndef DOBOTNET_SERIALINTERFACE_H
#define DOBOTNET_SERIALINTERFACE_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

class SerialInterface {
public:
	virtual void begin(long baud) = 0;
	virtual void end() = 0;
	virtual int available() = 0;
	virtual int read() = 0;
	virtual size_t write(uint8_t) = 0;
	virtual size_t write(const char* Str) = 0;
};

class SoftwareSerialWrapper : public SerialInterface {
private:
	SoftwareSerial* _serial = nullptr;

public:
	explicit SoftwareSerialWrapper(SoftwareSerial* serial) : _serial(serial) {}

	void begin(long baud) override {
		_serial->begin(baud);
	}

	void end() override {
		_serial->end();
	}

	int available() override {
		return _serial->available();
	}

	int read() override {
		return _serial->read();
	}

	size_t write(uint8_t data) override {
		return _serial->write(data);
	}

	size_t write(const char *Str) override {
		return _serial->write(Str);
	}
};

class HardwareSerialWrapper : public SerialInterface {
private:
	HardwareSerial* _serial = nullptr;

public:
	explicit HardwareSerialWrapper(HardwareSerial* serial) : _serial(serial) {}

	void begin(long baud) override {
		_serial->begin(baud);
	}

	void end() override {
		_serial->end();
	}

	int available() override {
		return _serial->available();
	}

	int read() override {
		return _serial->read();
	}

	size_t write(uint8_t data) override {
		return _serial->write(data);
	}

	size_t write(const char *Str) override {
		return _serial->write(Str);
	}
};

#endif //DOBOTNET_SERIALINTERFACE_H
