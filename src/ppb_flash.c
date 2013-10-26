#include <ppapi/c/private/ppb_flash.h>
#include <stddef.h>
#include <stdlib.h>
#include "trace.h"
#include "reverse_constant.h"

static
void
ppb_flash_set_instance_always_on_top(PP_Instance instance, PP_Bool on_top)
{
    trace_info("[PPB] {zilch} %s\n", __func__);
}

static
PP_Bool
ppb_flash_draw_glyphs(PP_Instance instance, PP_Resource pp_image_data,
                      const struct PP_BrowserFont_Trusted_Description *font_desc,
                      uint32_t color, const struct PP_Point *position, const struct PP_Rect *clip,
                      const float transformation[3][3], PP_Bool allow_subpixel_aa,
                      uint32_t glyph_count, const uint16_t glyph_indices[],
                      const struct PP_Point glyph_advances[])
{
    char *position_str = trace_point_as_string(position);
    char *clip_str = trace_rect_as_string(clip);
    trace_info("[PPB] {zilch} %s instance=%d, pp_image_data=%d, font_desc=%p, color=0x%x, "
               "position=%s, clip=%s, transformation={{%f,%f,%f},{%f,%f,%f},{%f,%f,%f}}, "
               "allow_subpixel_aa=%d, glyph_count=%u, glyph_indices=%p, glyph_advances=%p\n",
               __func__, instance, pp_image_data, font_desc, color, position_str, clip_str,
               transformation[0][0], transformation[0][1], transformation[0][2],
               transformation[1][0], transformation[1][1], transformation[1][2],
               transformation[2][0], transformation[2][1], transformation[2][2],
               allow_subpixel_aa, glyph_count, glyph_indices, glyph_advances);
    free(position_str);
    free(clip_str);
    return PP_TRUE;
}

static
struct PP_Var
ppb_flash_get_proxy_for_url(PP_Instance instance, const char *url)
{
    struct PP_Var var = {0};
    trace_info("[PPB] {zilch} %s instance=%d, url=%s\n", __func__, instance, url);
    return var;
}

static
int32_t
ppb_flash_navigate(PP_Resource request_info, const char *target, PP_Bool from_user_action)
{
    trace_info("[PPB] {zilch} %s request_info=%d, target=%s, from_user_action=%d\n", __func__,
               request_info, target, from_user_action);
    return 0;
}

static
double
ppb_flash_get_local_time_zone_offset(PP_Instance instance, PP_Time t)
{
    trace_info("[PPB] {zilch} %s instance=%d, t=%f\n", __func__, instance, t);
    return 0.0;
}

struct PP_Var
ppb_flash_get_command_line_args(PP_Module module)
{
    struct PP_Var var = {0};
    trace_info("[PPB] {zilch} %s module=%d\n", __func__, module);
    return var;
}

static
void
ppb_flash_preload_font_win(const void *logfontw)
{
    trace_info("[PPB] {zilch} %s logfontw=%p\n", __func__, logfontw);
}

static
PP_Bool
ppb_flash_is_rect_topmost(PP_Instance instance, const struct PP_Rect *rect)
{
    char *rect_str = trace_rect_as_string(rect);
    trace_info("[PPB] {zilch} %s instance=%d, rect=%s\n", __func__, instance, rect_str);
    free(rect_str);
    return PP_TRUE;
}

static
void
ppb_flash_update_activity(PP_Instance instance)
{
    trace_info("[PPB] {zilch} %s instance=%d\n", __func__, instance);
}

static
struct PP_Var
ppb_flash_get_setting(PP_Instance instance, PP_FlashSetting setting)
{
    trace_info("[PPB] {part} %s instance=%d, setting=%s\n", __func__, instance,
               reverse_pp_flash_setting(setting));
    struct PP_Var var = {0};

    switch (setting) {
    case PP_FLASHSETTING_3DENABLED:
        var.type = PP_VARTYPE_BOOL;
        var.value.as_bool = PP_TRUE;
        break;
    case PP_FLASHSETTING_INCOGNITO:
        // TODO: incognito?
        var.type = PP_VARTYPE_BOOL;
        var.value.as_bool = PP_FALSE;
        break;
    case PP_FLASHSETTING_STAGE3DENABLED:
        var.type = PP_VARTYPE_BOOL;
        var.value.as_bool = PP_TRUE;
        break;
    case PP_FLASHSETTING_LANGUAGE:
        // TODO: detect language
        var.type = PP_VARTYPE_STRING;
        var.value.as_id = (int64_t)(void*)"ru-RU";
        break;
    case PP_FLASHSETTING_NUMCORES:
        // TODO: check number of cores
        var.type = PP_VARTYPE_INT32;
        var.value.as_int = 2;
        break;
    case PP_FLASHSETTING_LSORESTRICTIONS:
        // TODO: select restrictions
        var.type = PP_VARTYPE_INT32;
        var.value.as_int = PP_FLASHLSORESTRICTIONS_IN_MEMORY;
        break;
    case PP_FLASHSETTING_STAGE3DBASELINEENABLED:
        // TODO: check if driver reliable enough
        var.type = PP_VARTYPE_BOOL;
        var.value.as_bool = PP_FALSE;
        break;
    }

    return var;
}

static
PP_Bool
ppb_flash_set_crash_data(PP_Instance instance, PP_FlashCrashKey key, struct PP_Var value)
{
    char *value_str = trace_var_as_string(value);
    trace_info("[PPB] {zilch} %s instance=%d, key=%d, value=%s\n", __func__, instance, key, value_str);
    free(value_str);
    return PP_TRUE;
}

static
int32_t
ppb_flash_enumerate_video_capture_devices(PP_Instance instance, PP_Resource video_capture,
                                          struct PP_ArrayOutput devices)
{
    trace_info("[PPB] {zilch} %s\n", __func__);
    return 0;
}


const struct PPB_Flash_13_0 ppb_flash_interface_13_0 = {
    .SetInstanceAlwaysOnTop = ppb_flash_set_instance_always_on_top,
    .DrawGlyphs = ppb_flash_draw_glyphs,
    .GetProxyForURL = ppb_flash_get_proxy_for_url,
    .Navigate = ppb_flash_navigate,
    .GetLocalTimeZoneOffset = ppb_flash_get_local_time_zone_offset,
    .GetCommandLineArgs = ppb_flash_get_command_line_args,
    .PreloadFontWin = ppb_flash_preload_font_win,
    .IsRectTopmost = ppb_flash_is_rect_topmost,
    .UpdateActivity = ppb_flash_update_activity,
    .GetSetting = ppb_flash_get_setting,
    .SetCrashData = ppb_flash_set_crash_data,
    .EnumerateVideoCaptureDevices = ppb_flash_enumerate_video_capture_devices
};
