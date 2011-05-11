/*
* Copyright (c) 2011 MLBA. All rights reserved.
*
* @MLBA_OPEN_LICENSE_HEADER_START@
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @MLBA_OPEN_LICENSE_HEADER_END@
*/


#ifndef QDISPATCH_SOURCE_H_
#define QDISPATCH_SOURCE_H_

#include <qobject.h>
#include <qrunnable.h>

#include "qdispatchglobal.h"
#include "../xdispatch/dispatch.h"

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QRunnable;
class QIODevice;
class QNetworkAccessManager;
class QDispatchQueue;
class QNetworkReply;

QT_MODULE(Dispatch)

class QDispatchSource;

/**
 Base class of all source types supported by QDispatchSource

 Subclass this to create your own sourcetype. Make sure to
 emit the ready signal whenever you want to indicate that
 your source is ready and the configured handler should be
 executed.
 */
class Q_DISPATCH_EXPORT QDispatchSourceType : public QObject {

	Q_OBJECT

protected:
	/**
	 @deprecated
	 */
	virtual void init(QThread*) = 0;

signals:
	/**
	 This signal shall be emitted each time the source the
	 type represents has news available. The passed QObject*
	 shall either be the new data available or a pointer to
	 another QObject associated with the source.

	 The pointer passed here can later be obtained by the handler
	 by using QDispatchSource::data()
	 */
	void ready(QObject* = NULL);

	friend class QDispatchSource;
};

/**
 Uses a signal emitted by a QObject as source and will dispatch
 the sources handler each time the signal is emitted. This source
 type does not provide any useful data when calling QDispatchSource::data()

 Make sure to delete the object used in here when deleting the source.
 */
class Q_DISPATCH_EXPORT QDispatchSourceTypeSignal : public QDispatchSourceType {

public:
	QDispatchSourceTypeSignal(QObject* sender, const char* signal);
	~QDispatchSourceTypeSignal();

protected:
	virtual void init(QThread*);

private:
	Q_DISABLE_COPY(QDispatchSourceTypeSignal);

};

/**
 Provides a source dispatching the handler each time the QIODevice has new
 data to be read. QDispatchSource::data() will return a pointer to the QIODevice
 that has new data to b read, normally this is the device you passed upon creating
 this type
 
 Deletion of the QIODevice is your own responsibility
 */
class Q_DISPATCH_EXPORT QDispatchSourceTypeIODevice : public QDispatchSourceType {

	Q_OBJECT

public:
	QDispatchSourceTypeIODevice(QIODevice*);
	~QDispatchSourceTypeIODevice();

protected:
	virtual void init(QThread*);

private slots:
	void avail();

private:
	Q_DISABLE_COPY(QDispatchSourceTypeIODevice);
	QIODevice* dev;
};

/**
 @see QDispatchSourceTypeIODevice

 This is equal to the QDispatchSourceTypeIODevice type and is provided as
 counterpart to QDispatchSourceTypeIODeviceWrite only.
 
 Deletion of the QIODevice is your own responsibility
 */
class Q_DISPATCH_EXPORT QDispatchSourceTypeIODeviceRead : public QDispatchSourceTypeIODevice {

	Q_OBJECT

public:
	QDispatchSourceTypeIODeviceRead(QIODevice*);

private:
	Q_DISABLE_COPY(QDispatchSourceTypeIODeviceRead);
};

/**
 Provides a source dispatching the handler each time the QIODevice has finished
 to write data. QDispatchSource::data() will return a pointer to the QIODevice
 that has written the data, normally this is the device you passed upon creating
 this type
 
 Deletion of the QIODevice is your own responsibility
 */
class Q_DISPATCH_EXPORT QDispatchSourceTypeIODeviceWrite : public QDispatchSourceType {

	Q_OBJECT

public:
	QDispatchSourceTypeIODeviceWrite(QIODevice*);
	~QDispatchSourceTypeIODeviceWrite();

protected:
	virtual void init(QThread*);

private slots:
	void finished(qint64);

private:
	Q_DISABLE_COPY(QDispatchSourceTypeIODeviceWrite);
	QIODevice* dev;
};

/**
 This type will dispatch the handler each time the given network manager finished
 a QNetworkReply. The finished reply can be obtained by using QDispatchSource::data()
 from within your handler.

 The descrution of the network manager is your own responsibility
 */
class Q_DISPATCH_EXPORT QDispatchSourceTypeNetworkManager : public QDispatchSourceType {

	Q_OBJECT

public:
	QDispatchSourceTypeNetworkManager(QNetworkAccessManager*);
	~QDispatchSourceTypeNetworkManager();

private slots:
	void finished(QNetworkReply*);

protected:
	void init(QThread*);

private:
	Q_DISABLE_COPY(QDispatchSourceTypeNetworkManager);
	QNetworkAccessManager* manager;
};

/**
 Provides a dispatch source. A dispatch source will wait for a specific
 ressource to become available or some other component to finish a current
 job and dispatch a specified handler upon completion.

 The currently supported QDispatchSourceTypes are:
	- QDispatchSourceTypeSignal
	- QDispatchSourceTypeQIODevice
	- QDispatchSourceTypeQIODeviceRead
	- QDispatchSourceTypeQIODeviceWrite
	- QDispatchSourceTypeNetworkManager

 You can easily add your own by subclassing QDispatchSourceType
 */
class Q_DISPATCH_EXPORT QDispatchSource : public QObject {

	Q_OBJECT

public:
	/**
	 Creates a new source waiting for the given type.
	 The passed type will be deleted as soon as the source is deleted
	 */
	QDispatchSource(QDispatchSourceType*);
	~QDispatchSource();
	/**
	 Sets the given runnable as handler that will be executed each time
	 the source has become ready.

	 @see data() and the source type's documentation on how to obtain
			information about the reason the source has become ready
	 */
	void setHandler(QRunnable*);
#ifdef XDISPATCH_HAS_BLOCKS
	/**
	 Sets the given block as handler that will be executed each time
	 the source has become ready.

	 @see data() and the source type's documentation on how to obtain
			information about the reason the source has become ready
	 */
	void setHandler(dispatch_block_t);
#endif
	/**
	 Sets the target, i.e. the queue the handler will be dispatched
	 on. Defaults to QDispatch::globalQueue().
	 */
	void setTarget(const QDispatchQueue&);
	/**
	 @returns the data associated to the current QDispatchSourceType. See
		the individual type documentations for details on the data available.

	 Call this to obtain data from within a handler while executing.
	 Calling this method from somewhere else than an executing handler
	 is undefined
	 */
	static QObject* data();

public slots:
	void resume();
	void suspend();

private slots:
	void signal(QObject* = NULL);

private:
	Q_DISABLE_COPY(QDispatchSource);
	class Private;
	Private* d;

};


QT_END_NAMESPACE
QT_END_HEADER

#endif /* QDISPATCH_SOURCE_H_ */
