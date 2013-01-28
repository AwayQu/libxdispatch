/*
* Copyright (c) 2008-2009 Apple Inc. All rights reserved.
* Copyright (c) 2011-2013 MLBA-Team. All rights reserved.
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


#include <QtCore/QTime>
#include <QtCore/QFile>
#include <QtDispatch/QtDispatch>

#include "Qt_tests.h"

/*
 Tests the source type IODevice
 */

extern "C" void Qt_dispatch_source_device(){
	char* argv = QString("test").toAscii().data();
	int argc = 1;
    QDispatchApplication app(argc,&argv);

        MU_BEGIN_TEST(Qt_dispatch_source_device);

	// TODO How? to test this?
	QFile f;
	QDispatchSource src(new QDispatchSourceTypeIODevice(&f));
	MU_PASS("");

	app.exec();
	MU_END_TEST;
}
