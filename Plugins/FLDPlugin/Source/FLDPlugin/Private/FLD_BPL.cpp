// Copyright Epic Games, Inc. All Rights Reserved.

#include "FLD_BPL.h"
#include "FLDPlugin.h"

typedef bool(*_getInvertedBool)(bool boolState); // Declare a method to store the DLL method getInvertedBool.
typedef int(*_getIntPlusPlus)(int lastInt); // Declare a method to store the DLL method getIntPlusPlus.
typedef float(*_getCircleArea)(float radius); // Declare a method to store the DLL method getCircleArea.
typedef char* (*_getCharArray)(char* parameterText); // Declare a method to store the DLL method getCharArray.
typedef float* (*_getVector4)(float x, float y, float z, float w); // Declare a method to store the DLL method getVector4.

typedef void (*_invertBool) ();
typedef bool (*_getBool) ();

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

typedef void (*_get_facial_landmarks)(int face_index, float*& arr_output, int& size);

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
_get_facial_landmarks m_get_facial_landmarks;

void* v_dllHandle;

bool UFLD_BPL::importDllAndDllFunctions(FString folder, FString name)
{
    FString filePath = *FPaths::ProjectContentDir() + folder + "/" + name;

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


            procName = "GetFacialLandmarks";
            m_get_facial_landmarks = (_get_facial_landmarks)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_facial_landmarks == NULL;

            return SomeWentWrong;
        }
    }
    return false;
}

int32 UFLD_BPL::InitOpenCV(int32 CamIndex, FString Folder, FString FaceDetectorFilePath, FString FaceMarkModelFilePath,
    int32 MouseWheelFieldWidth, int32 MouseWheelFieldHeight)
{
    if (m_init_open_cv != NULL)
    {
        const FString filePathCascade = *FPaths::ProjectContentDir() + Folder + "/" + FaceDetectorFilePath;
        const FString filePathModel = *FPaths::ProjectContentDir() + Folder + "/" + FaceMarkModelFilePath;
        return m_init_open_cv(CamIndex, TCHAR_TO_ANSI(*filePathCascade), TCHAR_TO_ANSI(*filePathModel), MouseWheelFieldWidth, MouseWheelFieldHeight);
    }

    return -1;
}

bool UFLD_BPL::IsEyeOpen(int32 FaceIndex, bool CheckLeftEye, float EAR)
{
    if (m_is_eye_open != NULL)
        return m_is_eye_open(FaceIndex, CheckLeftEye, EAR);
    return true;
}

bool UFLD_BPL::IsMouthOpen(int32 FaceIndex, float MAR)
{
    if (m_is_mouth_open != NULL)
        return m_is_mouth_open(FaceIndex, MAR);
    return false;
}

bool UFLD_BPL::CalculateFacialLandmarks()
{
    if (m_calculate_facial_landmarks != NULL)
        return m_calculate_facial_landmarks();
    return false;
}

bool UFLD_BPL::GetFrame(TArray<FColor>& Frame)
{
    int32 h = 0; int32 w = 0;
    GetFrameSize(w, h);
    if (Frame.Num() != h * w)
        Frame.Init(FColor(0, 0, 0, 255), h * w);

    if (m_get_frame != NULL)
    {
        unsigned char* FramePtr = nullptr;
        m_get_frame(FramePtr);

        // Copy Mat data to Data array
        for (int32 y = 0; y < h; y++)
        {
            for (int32 x = 0; x < w; x++)
            {
                int32 const i = x + (y * w);
                Frame[i].B = FramePtr[i * 3 + 0];
                Frame[i].G = FramePtr[i * 3 + 1];
                Frame[i].R = FramePtr[i * 3 + 2];
            }
        }

        return true;
    }
    return false;
}

bool UFLD_BPL::_StopOpenCV()
{
    if (m_stop_open_cv != NULL)
    {
        m_stop_open_cv();
        return true;
    }
    return false;
}

bool UFLD_BPL::SetIsNeedToShowBBox(bool NewValue)
{
    if (m_set_is_need_to_show_bbox != NULL)
    {
        m_set_is_need_to_show_bbox(NewValue);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetIsNeedToShowBBox()
{
    if (m_get_is_need_to_show_bbox != NULL)
    {
        return m_get_is_need_to_show_bbox();
    }
    return false;
}

bool UFLD_BPL::SetIsSelectedNosePositionForMouseControl(bool NewValue)
{
    if (m_set_is_selected_nose_position_for_mouse_control != NULL)
    {
        m_set_is_selected_nose_position_for_mouse_control(NewValue);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetIsSelectedNosePositionForMouseControl()
{
    if (m_get_is_selected_nose_position_for_mouse_control != NULL)
    {
        return m_get_is_selected_nose_position_for_mouse_control();
    }
    return false;
}

bool UFLD_BPL::SetMouseField(int32 x, int32 y, int32 width, int32 height)
{
    if (m_set_mouse_field != NULL)
    {
        m_set_mouse_field(x, y, width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetMouseField(int32& x, int32& y, int32& width, int32& height)
{
    if (m_get_mouse_field != NULL)
    {
        m_get_mouse_field(x, y, width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::IsCamOpened()
{
    if (m_is_camera_opened != NULL)
    {
        return m_is_camera_opened();
    }
    return false;
}

bool UFLD_BPL::GetFrameSize(int32& width, int32& height)
{
    if (m_get_frame_size != NULL)
    {
        m_get_frame_size(width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::ResizeFrame(int32 width, int32 height)
{
    if (m_resize_frame != NULL)
    {
        m_resize_frame(width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetFacialLandmarks(int32 face_index, TArray<FVector2D>& FacialLandmarks)
{
    int32 size_output = 0;
    float* facial_landmarks_ptr = nullptr;

    m_get_facial_landmarks(face_index, facial_landmarks_ptr, size_output);
    if (!facial_landmarks_ptr)
        return false;

    if (FacialLandmarks.Num() != 68)
    {
        FacialLandmarks.Init(FVector2D(0.f), 68);
    }

    for (int i = 0; i < 68; ++i)
    {
        FacialLandmarks[i].X = facial_landmarks_ptr[i * 2];
        FacialLandmarks[i].Y = facial_landmarks_ptr[i * 2 + 1];
    }
    return true;
}


#pragma endregion Method Calls


#pragma region Unload DLL

// If you love something  set it free.
void UFLD_BPL::freeDLL()
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

