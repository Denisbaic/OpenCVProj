// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenCV_BFL.h"


typedef bool(*_getInvertedBool)(bool boolState); // Declare a method to store the DLL method getInvertedBool.
typedef int(*_getIntPlusPlus)(int lastInt); // Declare a method to store the DLL method getIntPlusPlus.
typedef float(*_getCircleArea)(float radius); // Declare a method to store the DLL method getCircleArea.
typedef char* (*_getCharArray)(char* parameterText); // Declare a method to store the DLL method getCharArray.
typedef float* (*_getVector4)(float x, float y, float z, float w); // Declare a method to store the DLL method getVector4.

typedef void (*_invertBool) ();
typedef bool (* _getBool) ();

typedef int (*_init_opencv) (int cam_index, char* FaceDetectorCascadeFilePath, char* FaceMarkModelFilePath);
typedef void (*_show_frame) ();
typedef void (*_stop_opencv) ();

typedef int (*_init_dlib) (int cam_index, char* predictorFilePath);


typedef int(*_init_open_cv)(int cam_index,
    char* face_detector_cascade_file_path,
    char* face_mark_model_file_path,
    int mouse_wheel_field_width,
    int mouse_wheel_field_height);

typedef bool(*_is_eye_open)(int face_index, 
    bool check_left_eye, 
    float EAR);

typedef bool(*_is_mouth_open)(int face_index,
    float MAR);

typedef bool (*_bool_func_ptr)();
typedef void (*_void_func_ptr)();

typedef void (*_get_frame)(unsigned char*& OutputFrame);

typedef void (*_void_bool_ptr)(bool NewValue);

typedef bool (*_set_mouse_field)(int x, int y, int width, int height);
typedef bool (*_get_mouse_field)(int& x, int& y, int& width, int& height);
typedef void (*_void_two_int_ref)(int& width, int& height);


_getInvertedBool m_getInvertedBoolFromDll;
_getIntPlusPlus m_getIntPlusPlusFromDll;
_getCircleArea m_getCircleAreaFromDll;
_getCharArray m_getCharArrayFromDll;
_getVector4 m_getVector4FromDll;

_invertBool m_invertBoolFromDll;
_getBool m_getBoolFromDll;

_init_opencv m_init_opencv;
_show_frame m_show_frame;
_stop_opencv m_stop_opencv;

_init_dlib m_init_dlib;
_stop_opencv m_show_frame_dlib;


_init_open_cv m_init_open_cv;
_is_eye_open m_is_eye_open;
_is_mouth_open m_is_mouth_open;
_bool_func_ptr m_calculate_facial_landmarks;
_get_frame m_get_frame;
_void_func_ptr m_stop_open_cv;
_void_bool_ptr m_set_is_need_to_show_bbox;
_bool_func_ptr m_get_is_need_to_show_bbox;
_void_bool_ptr m_set_is_selected_nose_position_for_mouse_control;
_bool_func_ptr m_get_is_selected_nose_position_for_mouse_control;
_set_mouse_field m_set_mouse_field;
_get_mouse_field m_get_mouse_field;
_bool_func_ptr m_is_camera_opened;
_void_two_int_ref m_get_frame_size;
_void_two_int_ref m_resize_frame;
void* v_dllHandle;




#pragma region Load DLL

// Method to import a DLL.
bool UOpenCV_BFL::importDLL(FString folder, FString name)
{
    FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}
#pragma endregion Load DLL

#pragma region Import Methods

// Imports the method getInvertedBool from the DLL.
bool UOpenCV_BFL::importMethodGetInvertedBool()
{
    if (v_dllHandle != NULL)
    {
        m_getInvertedBoolFromDll = NULL;
        FString procName = "getInvertedBool";    // Needs to be the exact name of the DLL method.
        m_getInvertedBoolFromDll = (_getInvertedBool)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getInvertedBoolFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

// Imports the method getIntPlusPlus from the DLL.
bool UOpenCV_BFL::importMethodGetIntPlusPlus()
{
    if (v_dllHandle != NULL)
    {
        m_getIntPlusPlusFromDll = NULL;
        FString procName = "getIntPlusPlus";    // Needs to be the exact name of the DLL method.
        m_getIntPlusPlusFromDll = (_getIntPlusPlus)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getIntPlusPlusFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

// Imports the method getCircleArea from the DLL.
bool UOpenCV_BFL::importMethodGetCircleArea()
{
    if (v_dllHandle != NULL)
    {
        m_getCircleAreaFromDll = NULL;
        FString procName = "getCircleArea";    // Needs to be the exact name of the DLL method.
        m_getCircleAreaFromDll = (_getCircleArea)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getCircleAreaFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

// Imports the method getCharArray from the DLL.
bool UOpenCV_BFL::importMethodGetCharArray()
{
    if (v_dllHandle != NULL)
    {
        m_getCharArrayFromDll = NULL;
        FString procName = "getCharArray";    // Needs to be the exact name of the DLL method.
        m_getCharArrayFromDll = (_getCharArray)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getCharArrayFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

// Imports the method getVector4 from the DLL.
bool UOpenCV_BFL::importMethodGetVector4()
{
    if (v_dllHandle != NULL)
    {
        m_getVector4FromDll = NULL;
        FString procName = "getVector4";    // Needs to be the exact name of the DLL method.
        m_getVector4FromDll = (_getVector4)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getVector4FromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

bool UOpenCV_BFL::importMethodInvertBool()
{
    if (v_dllHandle != NULL)
    {
        m_invertBoolFromDll = NULL;
        FString procName = "InvertBool";    // Needs to be the exact name of the DLL method.
        m_invertBoolFromDll = (_invertBool)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_invertBoolFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

bool UOpenCV_BFL::importMethodGetBool()
{
    if (v_dllHandle != NULL)
    {
        m_getBoolFromDll = NULL;
        FString procName = "getBool";    // Needs to be the exact name of the DLL method.
        m_getBoolFromDll = (_getBool)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_getBoolFromDll != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}

void UOpenCV_BFL::InvertBoolFromDLL()
{
    if (m_invertBoolFromDll != NULL)
    {
        m_invertBoolFromDll(); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.

    }
}

bool UOpenCV_BFL::GetBoolFromDLL()
{
    if (m_getBoolFromDll != NULL)
    {
	    const bool out = static_cast<bool>(m_getBoolFromDll()); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
        return out;
    }
    return false;
}

#pragma endregion Import Methods

#pragma region Method Calls

// Calls the method getInvertedBoolFromDll that was imported from the DLL.
bool UOpenCV_BFL::getInvertedBoolFromDll(bool boolState)
{
    if (m_getInvertedBoolFromDll != NULL)
    {
        bool out = bool(m_getInvertedBoolFromDll(boolState)); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
        return out;
    }
    return boolState;    // Return an error.
}

// Calls the method m_getIntPlusPlusFromDll that was imported from the DLL.
int UOpenCV_BFL::getIntPlusPlusFromDll(int lastInt)
{
    if (m_getIntPlusPlusFromDll != NULL)
    {
        int out = int(m_getIntPlusPlusFromDll(lastInt)); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
        return out;
    }
    return -32202;    // Return an error.
}

// Calls the method m_getCircleAreaFromDll that was imported from the DLL.
float UOpenCV_BFL::getCircleAreaFromDll(float radius)
{
    if (m_getCircleAreaFromDll != NULL)
    {
        float out = float(m_getCircleAreaFromDll(radius)); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
        return out;
    }
    return -32202.0F;    // Return an error.
}

// Calls the method m_getCharArrayFromDLL that was imported from the DLL.
FString UOpenCV_BFL::getCharArrayFromDll(FString parameterText)
{
    if (m_getCharArrayFromDll != NULL)
    {
        char* parameterChar = TCHAR_TO_ANSI(*parameterText);

        char* returnChar = m_getCharArrayFromDll(parameterChar);

        return (ANSI_TO_TCHAR(returnChar));
    }
    return "Error: Method getCharArray was probabey not imported yet!";    // Return an error.
}

// Calls the method m_getVector4FromDll that was imported from the DLL.
FVector4 UOpenCV_BFL::getVector4FromDll(FVector4 vector4)
{
    if (m_getVector4FromDll != NULL)
    {
        float* vector4Array = m_getVector4FromDll(vector4.X, vector4.Y, vector4.Z, vector4.W);

        return FVector4(vector4Array[0], vector4Array[1], vector4Array[2], vector4Array[3]);
    }
    return FVector4(-32202.0F, -32202.0F, -32202.0F, -32202.0F);    // Return an error.
}

bool UOpenCV_BFL::importMethodInitOpenCV()
{
    if (v_dllHandle != NULL)
    {
        m_init_opencv = NULL;
        FString procName = "init_opencv";    // Needs to be the exact name of the DLL method.
        m_init_opencv = (_init_opencv)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_init_opencv != NULL)
        {
            return true;
        }
    }
    return false;
}

bool UOpenCV_BFL::importMethodShowFrame()
{
    if (v_dllHandle != NULL)
    {
        m_show_frame = NULL;
        FString procName = "show_frame";    // Needs to be the exact name of the DLL method.
        m_show_frame = (_show_frame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_show_frame != NULL)
        {
            return true;
        }
    }
    return false;
}

bool UOpenCV_BFL::importMethodStopOpenCV()
{
    if (v_dllHandle != NULL)
    {
        m_stop_opencv = NULL;
        FString procName = "stop_opencv";    // Needs to be the exact name of the DLL method.
        m_stop_opencv = (_stop_opencv)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_stop_opencv != NULL)
        {
            return true;
        }
    }
    return false;
}

int32 UOpenCV_BFL::startOpenCV(FString folder, FString nameCascade, FString nameModel)
{
    if (m_init_opencv != NULL)
    {
        FString filePathCascade = *FPaths::ProjectPluginsDir() + folder + "/" + nameCascade;
        FString filePathModel = *FPaths::ProjectPluginsDir() + folder + "/" + nameModel;

        int result = -1;

            result = m_init_opencv(0, TCHAR_TO_ANSI(*filePathCascade), TCHAR_TO_ANSI(*filePathModel));
    	

            //UE_LOG(LogTemp, Warning, TEXT("EXCEPTION HERE %s"), *FString(e.what()));
        

        return result;
    }
    return -1;    // Return an error.
}

void UOpenCV_BFL::showFrame()
{
    if (m_show_frame != NULL)
    {
        m_show_frame();
    }
}

void UOpenCV_BFL::stopOpenCV()
{
    if (m_stop_opencv != NULL)
    {
        m_stop_opencv();
    }
}

bool UOpenCV_BFL::importStartDLib()
{
    if (v_dllHandle != NULL)
    {
        m_init_dlib = NULL;
        FString procName = "init_dlib";    // Needs to be the exact name of the DLL method.
        m_init_dlib = (_init_dlib)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_init_dlib != NULL)
        {
            return true;
        }
    }
    return false;
}

bool UOpenCV_BFL::importMethodShowDLib()
{
    if (v_dllHandle != NULL)
    {
        m_show_frame_dlib = NULL;
        FString procName = "show_frame_dlib";    // Needs to be the exact name of the DLL method.
        m_show_frame_dlib = (_stop_opencv)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
        if (m_show_frame_dlib != NULL)
        {
            return true;
        }
    }
    return false;
}

int32 UOpenCV_BFL::startDLib(FString folder, FString predictorFilePath)
{
    if (m_init_dlib != NULL)
    {
        FString filePathCascade = *FPaths::ProjectPluginsDir() + folder + "/" + predictorFilePath;
    	
        return m_init_dlib(0,TCHAR_TO_ANSI(*filePathCascade));
    }
    return -1;
}

void UOpenCV_BFL::showFrameDLib()
{
    if (m_show_frame_dlib != NULL)
    {

        m_show_frame_dlib();
    }
}

void UOpenCV_BFL::stopDLib()
{
}

bool UOpenCV_BFL::importDllAndDllFunctions(FString folder, FString name)
{
    FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

    bool SomeWentWrong = false;

    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
            FString procName = "InitOpenCV";        	
            m_init_open_cv = (_init_open_cv)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_init_open_cv == NULL;

        	procName = "IsEyeOpen";
            m_is_eye_open = (_is_eye_open)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_eye_open == NULL;

        	procName = "IsMouthOpen";
            m_is_mouth_open = (_is_mouth_open)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_mouth_open == NULL;

        	procName = "CalculateFacialLandmarks";
            m_calculate_facial_landmarks = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_calculate_facial_landmarks == NULL;

        	procName = "GetFrame";
            m_get_frame = (_get_frame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_frame == NULL;

        	procName = "StopOpenCV";
            m_stop_open_cv = (_void_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_stop_open_cv == NULL;

        	procName = "SetIsNeedToShowBBox";
            m_set_is_need_to_show_bbox = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_is_need_to_show_bbox == NULL;

        	procName = "GetIsNeedToShowBBox";
            m_get_is_need_to_show_bbox = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_is_need_to_show_bbox == NULL;

        	procName = "SetIsSelectedNosePositionForMouseControl";
            m_set_is_selected_nose_position_for_mouse_control = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_is_selected_nose_position_for_mouse_control == NULL;

        	procName = "GetIsSelectedNosePositionForMouseControl";
            m_get_is_selected_nose_position_for_mouse_control = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_is_selected_nose_position_for_mouse_control == NULL;

            procName = "SetMouseField";
            m_set_mouse_field = (_set_mouse_field)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_mouse_field == NULL;

        	procName = "GetIsSelectedNosePositionForMouseControl";
            m_get_mouse_field = (_get_mouse_field)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_mouse_field == NULL;
        	
            procName = "IsCamOpened";
            m_is_camera_opened = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_camera_opened == NULL;

            procName = "GetFrameSize";
            m_get_frame_size = (_void_two_int_ref)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_frame_size == NULL;

            procName = "ResizeFrame";
            m_resize_frame = (_void_two_int_ref)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_resize_frame == NULL;
            
            return SomeWentWrong;
        }
    }
    return false;
}

int32 UOpenCV_BFL::InitOpenCV(int32 CamIndex,FString Folder, FString FaceDetectorFilePath, FString FaceMarkModelFilePath,
	int32 MouseWheelFieldWidth, int32 MouseWheelFieldHeight)
{
	if(m_init_open_cv != NULL)
	{
		const FString filePathCascade = *FPaths::ProjectPluginsDir() + Folder + "/" + FaceDetectorFilePath;
		const FString filePathModel = *FPaths::ProjectPluginsDir() + Folder + "/" + FaceMarkModelFilePath;
        return m_init_open_cv(CamIndex, TCHAR_TO_ANSI(*filePathCascade), TCHAR_TO_ANSI(*filePathModel), MouseWheelFieldWidth, MouseWheelFieldHeight);
	}
		
    return -1;
}

bool UOpenCV_BFL::IsEyeOpen(int32 FaceIndex, bool CheckLeftEye, float EAR)
{
    if (m_is_eye_open != NULL)
        return m_is_eye_open(FaceIndex,CheckLeftEye,EAR);
    return true;
}

bool UOpenCV_BFL::IsMouthOpen(int32 FaceIndex, float MAR)
{
    if (m_is_mouth_open != NULL)
        return m_is_mouth_open(FaceIndex, MAR);
    return false;
}

bool UOpenCV_BFL::CalculateFacialLandmarks()
{
    if (m_calculate_facial_landmarks != NULL)
        return m_calculate_facial_landmarks();
    return false;
}

bool UOpenCV_BFL::GetFrame(TArray<FColor>& Frame)
{
    int32 h = 0; int32 w = 0;
    GetFrameSize(w, h);
    if (Frame.Num() != h * w)
        Frame.Init(FColor(0, 0, 0, 255), h * w);
	
	if(m_get_frame != NULL)
	{
        unsigned char* FramePtr = nullptr;
        m_get_frame(FramePtr);

        // Copy Mat data to Data array
        for (int32 y = 0; y < h; y++)
        {
            for (int32 x = 0; x < w; x++)
            {
                int32 const i = x + (y *w);
                Frame[i].B = FramePtr[i * 3 + 0];
                Frame[i].G = FramePtr[i * 3 + 1];
                Frame[i].R = FramePtr[i * 3 + 2];
            }
        }
		
        return true;
	}	
    return false;
}

bool UOpenCV_BFL::_StopOpenCV()
{
    if (m_stop_open_cv != NULL)
    {
        m_stop_open_cv();
        return true;
    }
    return false;
}

bool UOpenCV_BFL::SetIsNeedToShowBBox(bool NewValue)
{
    if (m_set_is_need_to_show_bbox != NULL)
    {
        m_set_is_need_to_show_bbox(NewValue);
        return true;
    }
    return false;
}

bool UOpenCV_BFL::GetIsNeedToShowBBox()
{
    if (m_get_is_need_to_show_bbox != NULL)
    {
        return m_get_is_need_to_show_bbox();
    }
    return false;
}

bool UOpenCV_BFL::SetIsSelectedNosePositionForMouseControl(bool NewValue)
{
    if (m_set_is_selected_nose_position_for_mouse_control != NULL)
    {
        m_set_is_selected_nose_position_for_mouse_control(NewValue);
        return true;
    }
    return false;
}

bool UOpenCV_BFL::GetIsSelectedNosePositionForMouseControl()
{
    if (m_get_is_selected_nose_position_for_mouse_control != NULL)
    {
        return m_get_is_selected_nose_position_for_mouse_control();
    }
    return false;
}

bool UOpenCV_BFL::SetMouseField(int32 x, int32 y, int32 width, int32 height)
{
    if (m_set_mouse_field != NULL)
    {
        m_set_mouse_field( x,  y,  width,  height);
        return true;
    }
    return false;
}

bool UOpenCV_BFL::GetMouseField(int32& x, int32& y, int32& width, int32& height)
{
    if (m_get_mouse_field != NULL)
    {
        m_get_mouse_field(x, y, width, height);
        return true;
    }
    return false;
}

bool UOpenCV_BFL::IsCamOpened()
{
    if (m_is_camera_opened != NULL)
    {
        return m_is_camera_opened();
    }
    return false;
}

bool UOpenCV_BFL::GetFrameSize(int32& width, int32& height)
{
    if (m_get_frame_size != NULL)
    {
        m_get_frame_size(width, height);
    	return true;
    }
    return false;
}

bool UOpenCV_BFL::ResizeFrame(int32 width, int32 height)
{
    if (m_resize_frame != NULL)
    {
        m_resize_frame(width, height);
        return true;
    }
    return false;
}


#pragma endregion Method Calls


#pragma region Unload DLL

// If you love something  set it free.
void UOpenCV_BFL::freeDLL()
{
    if (v_dllHandle != NULL)
    {
        m_getInvertedBoolFromDll = NULL;
        m_getIntPlusPlusFromDll = NULL;
        m_getCircleAreaFromDll = NULL;
        m_getCharArrayFromDll = NULL;
        m_getVector4FromDll = NULL;

        FPlatformProcess::FreeDllHandle(v_dllHandle);
        v_dllHandle = NULL;
    }
}
#pragma endregion Unload DLL