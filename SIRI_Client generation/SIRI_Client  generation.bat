SET gSOAP_DIR=C:\external\gSOAP\

%gSOAP_DIR%\gsoap\bin\win32\wsdl2h -t%gSOAP_DIR%\gsoap\gsoap\typemap.dat -oSIRI.h ..\..\SIRI-2.0o-xsd\siri_wsConsumer-Document.wsdl 
echo *****Lines 46957 and 52562 of SIRI.h may need to be commented if find en error *****
PAUSE
%gSOAP_DIR%\gsoap\bin\win32\soapcpp2 -c++11 -j -I%gSOAP_DIR%\gsoap\import -I%gSOAP_DIR%\gsoap\gsoap SIRI.h
PAUSE