TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	Base_Types/Array_Variable.cpp \
	Base_Types/Bool_Variable.cpp \
	Base_Types/Float_Variable.cpp \
	Base_Types/Int_Variable.cpp \
	Base_Types/String_Variable.cpp \
	OMFL_Reader.cpp \
        Type_Manager.cpp \
	Base_Types/Variable_Base.cpp \
        Variable_Manager.cpp \
        main.cpp \
	parser.cpp \
	test.cpp

HEADERS += \
    Base_Types/Array_Variable.h \
    Base_Types/Bool_Variable.h \
    Base_Types/Float_Variable.h \
    Base_Types/Int_Variable.h \
    Base_Types/String_Variable.h \
    OMFL_Reader.h \
    Type_Manager.h \
    Base_Types/Variable_Base.h \
    Variable_Manager.h \
    parser.h

DISTFILES += \
	build/file.omfl
