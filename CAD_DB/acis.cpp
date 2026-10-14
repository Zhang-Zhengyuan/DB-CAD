#include <acis/include/kernapi.hxx>
#include <acis/include/license.hxx>
#include <acis/include/spa_unlock_result.hxx>

#include "acis.hxx"

// 官方 2026 许可密钥（Haihe Laboratory，3D ACIS Modeler 等）。
// 宏 SPATIAL_LICENSE 由 spatial_license.h 定义，该文件不在版本控制内发布。
#include "spatial_license.h"

// -------------------------------------------------------------------------------------------------------
// ACIS 环境初始化和结束函数

void unlock_license() {
    spa_unlock_result out = spa_unlock_products(SPATIAL_LICENSE);
    if (out.get_state() == SPA_UNLOCK_PASS_WARN) { // insert your warning notification code here
        ;
    } else if (out.get_state() != SPA_UNLOCK_PASS) { // insert your error handling code here
        ;
    }
}

// 返回初始化级别：1-基础，2-建模引擎，0-失败。
int initialize_acis() {
    outcome result;

    int level = 0;
    // 基础初始化将创建线程本地存储和线程ID。

    if (initialize_base()) {  // 在初始化ACIS时传递该默认配置
        ++level;
        outcome result = api_start_modeller(0);
        if (result.ok()) {
            ++level;
            // 在此添加通用ACIS状态。
        }
    }
    if (level < 3) unlock_license();
    return level;
}

void terminate_acis(int level) {
    // 终止ACIS
    if (level > 1) {
        // 终止之前必须先销毁该outcome对象，因为访问了线程本地存储。
        outcome result = api_stop_modeller();
        if (result.ok() != FALSE) {
            --level;
        }
    }

    if (level > 0) {
        if (terminate_base()) {
            --level;
        }
    }
}

std::string process(outcome& result) {
    if (!result.ok()) {
        std::string err(find_err_mess(result.error_number()));
        std::string errW(err.begin(), err.end());
        return errW;
    }
    err_mess_type* warnings = nullptr;
    int nwarn = get_warnings(warnings);
    for (int i = 0; i < nwarn; ++i) {
        printf("Warning %d : %s\n", warnings[i], find_err_mess(warnings[i]));
    }
    return "";
}
