SET gSOAP_DIR=C:\external\gSOAP\

echo This script requires gSOAP library to be installed in C:\external\gSOAP

%gSOAP_DIR%\gsoap\bin\win32\wsdl2h -t%gSOAP_DIR%\gsoap\typemap.dat -oSIRI.h ..\SIRI-2.0o-xsd\siri_wsConsumer-Document.wsdl 
PAUSE
%gSOAP_DIR%\gsoap\bin\win32\soapcpp2 -c++11 -j -I%gSOAP_DIR%\gsoap\import -I%gSOAP_DIR%\gsoap SIRI.h
PAUSE