// Copyright Epic Games, Inc. All Rights Reserved.

#include "FLD_BPL.h"

#define FLD_NOSE_INDEX 30

typedef int(*_init_open_cv)(int cam_index,
    char* face_detector_config,
    char* face_detector_weights,
    char* face_mark_model_file_path,
    int mouse_wheel_field_width,
    int mouse_wheel_field_height);

//typedef void(*_void_char_ptr)(char* folder);

typedef bool(*_is_eye_open)(int face_index,
    bool check_left_eye,
    float EAR,float& CurrentEAR);

typedef bool(*_is_mouth_open)(int face_index,
    float MAR, float& CurrentMAR);

typedef bool(*_is_eyebrowns_raised)(int face_index, float BAR, float& current_bar);

typedef bool (*_bool_func_ptr)();
typedef void (*_void_func_ptr)();

typedef void (*_get_frame)(unsigned char*& OutputFrame);

typedef void (*_void_bool_ptr)(bool NewValue);

typedef void (*_set_mouse_field)(int x, int y, int width, int height);
typedef void (*_get_mouse_field)(int& x, int& y, int& width, int& height);
typedef void (*_void_two_int_ref)(int& width, int& height);
typedef void (*_void_two_int)(int width, int height);

typedef void (*_get_facial_landmarks)(int face_index, float*& arr_output, int& size);

typedef void (*_void_three_float)(float R, float G, float B);
typedef void (*_void_three_float_ref)(float& R, float& G, float& B);

_init_open_cv m_init_open_cv= nullptr;
//_void_char_ptr m_load_data_set = nullptr;
_is_eye_open m_is_eye_open= nullptr;
_is_mouth_open m_is_mouth_open= nullptr;
_is_eyebrowns_raised m_is_eyebrowns_raised = nullptr;
_bool_func_ptr m_calculate_facial_landmarks= nullptr;
_get_frame m_get_frame= nullptr;
_void_func_ptr m_stop_open_cv= nullptr;
_void_bool_ptr m_set_is_need_to_show_bbox= nullptr;
_bool_func_ptr m_get_is_need_to_show_bbox= nullptr;
_void_bool_ptr m_set_is_selected_nose_position_for_mouse_control= nullptr;
_bool_func_ptr m_get_is_selected_nose_position_for_mouse_control= nullptr;
_set_mouse_field m_set_mouse_field= nullptr;
_get_mouse_field m_get_mouse_field= nullptr;
_bool_func_ptr m_is_camera_opened= nullptr;
_void_two_int_ref m_get_frame_size= nullptr;
_void_two_int m_resize_frame= nullptr;
_get_facial_landmarks m_get_facial_landmarks= nullptr;
_void_three_float m_set_ui_color = nullptr;
_void_three_float_ref m_get_ui_color = nullptr;
//_void_bool_ptr m_set_test_mode = nullptr;
//_void_bool_ptr m_next_frame = nullptr;
void* v_dllHandle= nullptr;

bool UFLD_BPL::importDllAndDllFunctions(FString folder, FString name)
{
    FString filePath = *FPaths::ProjectContentDir() + folder + "/" + name;

    bool SomeWentWrong = false;

    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != nullptr)
        {
            FString procName = "InitOpenCV";
            m_init_open_cv = (_init_open_cv)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_init_open_cv == nullptr;

            //procName = "LoadDataSet";
            //m_load_data_set = (_void_char_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            //SomeWentWrong = SomeWentWrong || m_load_data_set == nullptr;
        	
            procName = "IsEyeOpen";
            m_is_eye_open = (_is_eye_open)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_eye_open == nullptr;

            procName = "IsMouthOpen";
            m_is_mouth_open = (_is_mouth_open)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_mouth_open == nullptr;
                    	
            procName = "IsEyebrowsRaised";
            m_is_eyebrowns_raised = (_is_eyebrowns_raised)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_eyebrowns_raised == nullptr;
        	
            procName = "CalculateFacialLandmarks";
            m_calculate_facial_landmarks = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_calculate_facial_landmarks == nullptr;

            procName = "GetFrame";
            m_get_frame = (_get_frame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_frame == nullptr;

            procName = "StopOpenCV";
            m_stop_open_cv = (_void_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_stop_open_cv == nullptr;

            procName = "SetIsNeedToShowBBox";
            m_set_is_need_to_show_bbox = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_is_need_to_show_bbox == nullptr;

            procName = "GetIsNeedToShowBBox";
            m_get_is_need_to_show_bbox = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_is_need_to_show_bbox == nullptr;

            procName = "SetIsSelectedNosePositionForMouseControl";
            m_set_is_selected_nose_position_for_mouse_control = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_is_selected_nose_position_for_mouse_control == nullptr;

            procName = "GetIsSelectedNosePositionForMouseControl";
            m_get_is_selected_nose_position_for_mouse_control = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_is_selected_nose_position_for_mouse_control == nullptr;

            procName = "SetMouseField";
            m_set_mouse_field = (_set_mouse_field)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_mouse_field == nullptr;

            procName = "GetMouseField";
            m_get_mouse_field = (_get_mouse_field)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_mouse_field == nullptr;
        	
            procName = "IsCamOpened";
            m_is_camera_opened = (_bool_func_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_is_camera_opened == nullptr;

            procName = "GetFrameSize";
            m_get_frame_size = (_void_two_int_ref)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_frame_size == nullptr;

            procName = "ResizeFrame";
            m_resize_frame = (_void_two_int)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_resize_frame == nullptr;
            
            procName = "GetFacialLandmarks";
            m_get_facial_landmarks = (_get_facial_landmarks)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_facial_landmarks == nullptr;

            procName = "SetUIColor";
            m_set_ui_color = (_void_three_float)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_facial_landmarks == nullptr;

            procName = "GetUIColor";
            m_get_ui_color = (_void_three_float_ref)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_get_facial_landmarks == nullptr;

            /*procName = "SetTestMode";
            m_set_test_mode = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_set_test_mode == nullptr;

            procName = "NextFrame";
            m_next_frame = (_void_bool_ptr)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
            SomeWentWrong = SomeWentWrong || m_next_frame == nullptr;*/
            
            return SomeWentWrong;
        }
    }
    return false;
}

int32 UFLD_BPL::InitOpenCV(int32 CamIndex, FString Folder, FString FaceDetectorConfigFilePath, FString FaceDetectorWeightsFilePath, FString FaceMarkModelFilePath,
    int32 MouseWheelFieldWidth, int32 MouseWheelFieldHeight)
{
    if (m_init_open_cv != nullptr)
    {
        const FString filePathConfig = *FPaths::ProjectContentDir() + Folder + "/" + FaceDetectorConfigFilePath;
        const FString filePathWeights = *FPaths::ProjectContentDir() + Folder + "/" + FaceDetectorWeightsFilePath;
        const FString filePathModel = *FPaths::ProjectContentDir() + Folder + "/" + FaceMarkModelFilePath;
        return m_init_open_cv(CamIndex, TCHAR_TO_ANSI(*filePathConfig), TCHAR_TO_ANSI(*filePathWeights), TCHAR_TO_ANSI(*filePathModel), MouseWheelFieldWidth, MouseWheelFieldHeight);
    }

    return -1;
}
/*
bool UFLD_BPL::LoadDataSet(FString Folder)
{
    if (m_load_data_set != nullptr)
    {
        const FString FolderPath = *FPaths::ProjectContentDir() + Folder + "/";
        m_load_data_set(TCHAR_TO_ANSI(*FolderPath));

        return true;
    }
    return true;
}
*/
bool UFLD_BPL::IsEyeOpen(float& CurrentEAR, int32 FaceIndex, bool CheckLeftEye, float EAR)
{
    if (m_is_eye_open != nullptr)
    {
        auto const returned_var = m_is_eye_open(FaceIndex, CheckLeftEye, EAR, CurrentEAR);
    	//if(!CheckLeftEye)
			//UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentEAR);
        return returned_var;
    }
    
	
    return true;
}

bool UFLD_BPL::IsMouthOpen(float& CurrentMAR, int32 FaceIndex, float MAR)
{
    if (m_is_mouth_open != nullptr)
    {
        auto const returned_var = m_is_mouth_open(FaceIndex, MAR, CurrentMAR);
        UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentMAR);
        return returned_var;
    }
        
    return false;
}

bool UFLD_BPL::IsEyebrowsRaised(float& CurrentBAR, int32 FaceIndex, float BAR)
{
    if (m_is_eyebrowns_raised != nullptr)
        return m_is_eyebrowns_raised(FaceIndex, BAR,CurrentBAR);
    return false;    
}

bool UFLD_BPL::CalculateFacialLandmarks()
{
    if (m_calculate_facial_landmarks != nullptr)
        return m_calculate_facial_landmarks();
    return false;
}

bool UFLD_BPL::GetFrame(TArray<FColor>& Frame)
{
    int32 h = 0; int32 w = 0;
    if (!GetFrameSize(w, h))
        return false;
    if (Frame.Num() != h * w)
        Frame.Init(FColor(0, 0, 0, 255), h * w);

    if (m_get_frame != nullptr)
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
    if (m_stop_open_cv != nullptr)
    {
        m_stop_open_cv();
        return true;
    }
    return false;
}

bool UFLD_BPL::SetIsNeedToShowBBox(bool NewValue)
{
    if (m_set_is_need_to_show_bbox != nullptr)
    {
        m_set_is_need_to_show_bbox(NewValue);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetIsNeedToShowBBox()
{
    if (m_get_is_need_to_show_bbox != nullptr)
    {
        return m_get_is_need_to_show_bbox();
    }
    return false;
}

bool UFLD_BPL::SetIsSelectedNosePositionForMouseControl(bool NewValue)
{
    if (m_set_is_selected_nose_position_for_mouse_control != nullptr)
    {
        m_set_is_selected_nose_position_for_mouse_control(NewValue);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetIsSelectedNosePositionForMouseControl()
{
    if (m_get_is_selected_nose_position_for_mouse_control != nullptr)
    {
        return m_get_is_selected_nose_position_for_mouse_control();
    }
    return false;
}

bool UFLD_BPL::SetMouseField(int32 x, int32 y, int32 width, int32 height)
{
    if (m_set_mouse_field != nullptr)
    {
        m_set_mouse_field(x, y, width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetMouseField(int32& x, int32& y, int32& width, int32& height)
{
    if (m_get_mouse_field != nullptr)
    {
        m_get_mouse_field(x, y, width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::IsCamOpened()
{
    if (m_is_camera_opened != nullptr)
    {
        return m_is_camera_opened();
    }
    return false;
}

bool UFLD_BPL::GetFrameSize(int32& width, int32& height)
{
    if (m_get_frame_size != nullptr)
    {
        m_get_frame_size(width, height);
        return true;
    }
    return false;
}

bool UFLD_BPL::ResizeFrame(int32 width, int32 height)
{
    if (m_resize_frame != nullptr)
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

    if (!m_get_facial_landmarks)
        return false;

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

bool UFLD_BPL::SetUIColor(FColor UIColor)
{
    if (m_set_ui_color != nullptr)
    {
        m_set_ui_color(UIColor.R, UIColor.G, UIColor.B);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetUIColor(float& R, float& G, float& B)
{
    if (m_get_ui_color != nullptr)
    {
        m_get_ui_color(R, G, B);
        return true;
    }
    return false;
}

bool UFLD_BPL::GetMouseDirection(int32 face_index, FVector2D& mouse_dir_out, bool IsNeedToRecalculate)
{
    if (!GetIsSelectedNosePositionForMouseControl())
        return false;
	
    if (!m_get_facial_landmarks)
        return false;
	
    if (IsNeedToRecalculate)
        CalculateFacialLandmarks();

    int32 size_output = 0;
    float* facial_landmarks_ptr = nullptr;

    m_get_facial_landmarks(face_index, facial_landmarks_ptr, size_output);
    if (!facial_landmarks_ptr)
        return false;

    FVector2D const NosePosition(facial_landmarks_ptr[FLD_NOSE_INDEX * 2], facial_landmarks_ptr[FLD_NOSE_INDEX * 2 + 1]);

    int32 x =0, y =0, width =0, height =0;
    GetMouseField(x, y, width, height);
    FVector2D const MouseFieldCenter(x + width / 2.f, y + height / 2.f);

    mouse_dir_out = NosePosition - MouseFieldCenter;
    return true;
}
/*
bool UFLD_BPL::SetTestMode(bool IsTestMode)
{
    if (m_set_test_mode != nullptr)
    {
        m_set_test_mode(IsTestMode);
        return true;
    }
    return false;
}

bool UFLD_BPL::NextFrame(bool GetOppositeFrame)
{
    if (m_next_frame != nullptr)
    {
        m_next_frame(GetOppositeFrame);
        return true;
    }
    return false;
}
*/
#pragma endregion Method Calls


#pragma region Unload DLL

// If you love something  set it free.
void UFLD_BPL::freeDLL()
{
    if (v_dllHandle != nullptr)
    {
        _StopOpenCV();
    	
        m_init_open_cv = nullptr;
        m_is_eye_open = nullptr;
        m_is_mouth_open = nullptr;
        m_is_eyebrowns_raised = nullptr;
        m_calculate_facial_landmarks = nullptr;
        m_get_frame = nullptr;
        m_stop_open_cv = nullptr;
        m_set_is_need_to_show_bbox = nullptr;
        m_get_is_need_to_show_bbox = nullptr;
        m_set_is_selected_nose_position_for_mouse_control = nullptr;
        m_get_is_selected_nose_position_for_mouse_control = nullptr;    	
        m_set_mouse_field = nullptr;
        m_get_mouse_field = nullptr;
        m_is_camera_opened = nullptr;
        m_resize_frame = nullptr;
        m_get_facial_landmarks = nullptr;
        m_set_ui_color = nullptr;
        m_get_ui_color = nullptr;   	

        FPlatformProcess::FreeDllHandle(v_dllHandle);
        v_dllHandle = nullptr;
    }
}
#pragma endregion Unload DLL