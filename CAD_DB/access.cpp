#include "common.hxx"
#include "access.hxx"
#include <acis/include/api.hxx>
#include <acis/include/kernapi.hxx>
#include <acis/include/alltop.hxx>
#include <acis/include/fileinfo.hxx>
#include <acis/include/att_tag.hxx>
#include <cstdio>
#include <string>

void acis_save_entity_list(const ENTITY_LIST &elist, const char *file_name, int major_version, int minor_version,
                           int text_mode) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE *save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_save_entity_list(FILE *file, const ENTITY_LIST &elist, int major_version, int minor_version, int text_mode) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    if (!file) {
        myerror("打开文件失败，文件句柄为空");
    } else {
        result = api_save_entity_list(file, text_mode, elist);
        fflush(file);
    }
    API_NOP_END;
}

void acis_get_noattrib_toplevel_active_entities(ENTITY_LIST &elist, HISTORY_STREAM *hs) {
    if (hs == NULL) {
        api_get_default_history(hs);
    }
    api_get_active_entities(hs, elist, 1); // 参数1表示只获取顶级实体

    // 去除（不保存）annotation、attrib等类型实体
    elist.init();
    ENTITY *tmpentity = nullptr;
    while (tmpentity = elist.next()) {
        if (is_ANNOTATION(tmpentity) || is_ATTRIB_TAG(tmpentity)) {
            elist.remove(tmpentity);
        }
    }
}

void acis_save_noattrib_toplevel_active_entities(const char *file_name, int major_version, int minor_version,
                                                 int text_mode, HISTORY_STREAM *hs) {
    ENTITY_LIST elist;
    acis_get_noattrib_toplevel_active_entities(elist, hs);
    acis_save_entity_list(elist, file_name, major_version, minor_version, text_mode);
}

void acis_save_history(const char *file_name, int major_version, int minor_version, int text_mode, HISTORY_STREAM *hs) {
    API_NOP_BEGIN;
    api_save_version(major_version, minor_version);
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("SimpleApi");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);
    FILE *save_file;
    fopen_s(&save_file, file_name, "wb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_save_history(save_file, text_mode, hs);
        fclose(save_file);
    }
    API_NOP_END;
}

void acis_restore_entity_list(ENTITY_LIST &elist, const char *file_name, int major_version, int minor_version,
                              int text_mode) {
    API_BEGIN;
    api_save_version(major_version, minor_version);
    FILE *save_file;
    fopen_s(&save_file, file_name, "rb");
    if (!save_file) {
        myerror("打开文件失败，文件名为" + std::string(file_name));
    } else {
        result = api_restore_entity_list(save_file, text_mode, elist);
        fclose(save_file);
    }
    API_END;
}

void acis_restore_entity_list(ENTITY_LIST &elist, FILE *file, int major_version, int minor_version, int text_mode) {
    API_BEGIN;
    api_save_version(major_version, minor_version);
    if (!file) {
        myerror("打开文件失败，文件句柄为空");
    } else {
        result = api_restore_entity_list(file, text_mode, elist);
    }
    API_END;
}

