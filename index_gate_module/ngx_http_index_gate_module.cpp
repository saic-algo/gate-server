extern "C" {  
#include <ngx_config.h>  
#include <ngx_core.h>  
#include <ngx_http.h>
}  
#include <ctime>
#include <cstdio>
#include <cstring>
#include <map>
#include <json/json.h>
#include "gate_registry.h"
using namespace std;

typedef struct {  
    ngx_str_t output_words;
    ngx_str_t cid;
    ngx_str_t lng;
    ngx_str_t lat;
    ngx_str_t status;
} ngx_http_index_gate_loc_conf_t;  
  
// To process Index_Gate command arguments  
static char* ngx_http_index_gate(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char* ngx_http_index_gate_cid(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char* ngx_http_index_gate_lng(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char* ngx_http_index_gate_lat(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char* ngx_http_index_gate_status(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
  
// Allocate memory for Index_Gate command  
static void* ngx_http_index_gate_create_loc_conf(ngx_conf_t* cf);  
  
// Copy Index_Gate argument to another place  
static char* ngx_http_index_gate_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);  
  
// Structure for the Index_Gate command  
static ngx_command_t ngx_http_index_gate_commands[] = {  
    {  
        ngx_string("index_gate"), // The command name  
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,  
        ngx_http_index_gate, // The command handler  
        NGX_HTTP_LOC_CONF_OFFSET,  
        offsetof(ngx_http_index_gate_loc_conf_t, output_words),  
        NULL  
    },  
    {  
        ngx_string("index_gate_cid"), // The command name  
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,  
        ngx_http_index_gate_cid, // The command handler  
        NGX_HTTP_LOC_CONF_OFFSET,  
        offsetof(ngx_http_index_gate_loc_conf_t, cid),  
        NULL  
    },  
    {  
        ngx_string("index_gate_lng"), // The command name  
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,  
        ngx_http_index_gate_lng, // The command handler  
        NGX_HTTP_LOC_CONF_OFFSET,  
        offsetof(ngx_http_index_gate_loc_conf_t, lng),  
        NULL  
    },  
    {  
        ngx_string("index_gate_lat"), // The command name  
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,  
        ngx_http_index_gate_lat, // The command handler  
        NGX_HTTP_LOC_CONF_OFFSET,  
        offsetof(ngx_http_index_gate_loc_conf_t, lat),  
        NULL  
    },  
    {  
        ngx_string("index_gate_status"), // The command name  
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_http_index_gate_status, // The command handler  
        NGX_HTTP_LOC_CONF_OFFSET,  
        offsetof(ngx_http_index_gate_loc_conf_t, status),  
        NULL  
    },  
    ngx_null_command  
};  
  
// Structure for the HelloWorld context  
static ngx_http_module_t ngx_http_index_gate_module_ctx = {  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    ngx_http_index_gate_create_loc_conf,  
    ngx_http_index_gate_merge_loc_conf  
};  
  
// Structure for the HelloWorld module, the most important thing  
ngx_module_t ngx_http_index_gate_module = {  
    NGX_MODULE_V1,  
    &ngx_http_index_gate_module_ctx,  
    ngx_http_index_gate_commands,  
    NGX_HTTP_MODULE,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NGX_MODULE_V1_PADDING  
};  
  
static ngx_int_t cid_index;
static ngx_int_t lat_index;
static ngx_int_t lng_index;
static ngx_int_t status_index;
static GateRegistry registry;

static ngx_int_t update_car_position(ngx_http_request_t* r) {
    ngx_int_t rc;
    ngx_buf_t* b;
    ngx_chain_t out;
    u_char ngx_output_string[1024] = {0};
    char ngx_lat_str[16] = {0};
    char ngx_lng_str[16] = {0};
    char ngx_cid_str[40] = {0};
    std::string cid;
    double lng;
    double lat;
    ngx_http_variable_value_t *value;
    ngx_uint_t content_length = 0;

    value = ngx_http_get_indexed_variable(r,cid_index);
    if (value != NULL && !value->not_found){
        memcpy(ngx_cid_str, value->data, sizeof(u_char) * value->len);
        cid = ngx_cid_str;
    }

    value = ngx_http_get_indexed_variable(r,lng_index);
    if (value != NULL && !value->not_found){
        memcpy(ngx_lng_str, value->data, sizeof(u_char) * value->len);
        sscanf(ngx_lng_str, "%lf", &lng);
    }

    value = ngx_http_get_indexed_variable(r,lat_index);
    if (value != NULL && !value->not_found){
        memcpy(ngx_lat_str, value->data, sizeof(u_char) * value->len);
        sscanf(ngx_lat_str, "%lf", &lat);
    }

    registry.UpdateCarPosition(cid,lat,lng,time(NULL));

    ngx_sprintf(ngx_output_string, "cid:%s\nlat:%.4f\nlng:%.4f\n", cid.c_str(), lat, lng);
    // ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "update_car_position is called! %s", ngx_output_string);

    content_length = ngx_strlen(ngx_output_string);

    r->headers_out.content_type.len = sizeof("text/plain") - 1;  
    r->headers_out.content_type.data = (u_char*)"text/plain";  
  

    b = (ngx_buf_t*)ngx_pcalloc(r->pool, sizeof(ngx_buf_t));  

    out.buf = b;  
    out.next = NULL;  
  
    b->pos = ngx_output_string;
    b->last = ngx_output_string + content_length;
    b->memory = 1;  
    b->last_buf = 1;
    
    r->headers_out.status = NGX_HTTP_OK;  
    r->headers_out.content_length_n = content_length;  
    rc = ngx_http_send_header(r);  
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {  
        return rc;  
    }  
  
    return ngx_http_output_filter(r, &out);
}

static ngx_int_t update_car_status(ngx_http_request_t* r) {
    ngx_int_t rc;
    ngx_buf_t* b;
    ngx_chain_t out;
    u_char ngx_output_string[1024] = {0};
    char ngx_status_str[4] = {0};
    char ngx_cid_str[40] = {0};
    std::string cid;
    int status;
    ngx_http_variable_value_t *value;
    ngx_uint_t content_length = 0;

    value = ngx_http_get_indexed_variable(r,cid_index);
    if (value != NULL && !value->not_found){
        memcpy(ngx_cid_str, value->data, sizeof(u_char) * value->len);
        cid = ngx_cid_str;
    }

    value = ngx_http_get_indexed_variable(r,status_index);
    if (value != NULL && !value->not_found){
        memcpy(ngx_status_str, value->data, sizeof(u_char) * value->len);
        sscanf(ngx_status_str, "%d", &status);
    }

    registry.UpdateCarStatus(cid,status);

    ngx_sprintf(ngx_output_string, "cid:%s\nstatus:%d\n", cid.c_str(), status);
    // ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "update_car_status is called! %s", ngx_output_string);

    content_length = ngx_strlen(ngx_output_string);

    r->headers_out.content_type.len = sizeof("text/plain") - 1;  
    r->headers_out.content_type.data = (u_char*)"text/plain";  
  

    b = (ngx_buf_t*)ngx_pcalloc(r->pool, sizeof(ngx_buf_t));  

    out.buf = b;  
    out.next = NULL;  
  
    b->pos = ngx_output_string;
    b->last = ngx_output_string + content_length;
    b->memory = 1;  
    b->last_buf = 1;
    
    r->headers_out.status = NGX_HTTP_OK;  
    r->headers_out.content_length_n = content_length;  
    rc = ngx_http_send_header(r);  
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {  
        return rc;  
    }  
  
    return ngx_http_output_filter(r, &out);
}

u_char ngx_out_json_string[10000000];

static ngx_int_t post_car_positions(ngx_http_request_t* r) {
    ngx_int_t rc;
    ngx_buf_t* b;
    ngx_chain_t out;
    ngx_uint_t content_length = 0;

    std::map<std::string, CarInfo> tmap;
    registry.getMap(tmap);
    Json::Value array;
    for(auto &it:tmap){
      //balala
      Json::Value v;
      v.append(it.second.cid);
      v.append(it.second.lat);
      v.append(it.second.lng);
      array.append(v);
      // std::string t=v.toStyledString();
    }
    Json::Value points;
    points["points"]=array;
    std::string outs=points.toStyledString();

    ngx_sprintf(ngx_out_json_string, "%s", outs.c_str());
    content_length = ngx_strlen(ngx_out_json_string);

    r->headers_out.content_type.len = sizeof("application/json") - 1;  
    r->headers_out.content_type.data = (u_char*)"application/json";  
  

    b = (ngx_buf_t*)ngx_pcalloc(r->pool, sizeof(ngx_buf_t));  

    out.buf = b;  
    out.next = NULL;  
  
    b->pos = ngx_out_json_string;
    b->last = ngx_out_json_string + content_length;
    b->memory = 1;  
    b->last_buf = 1;
    
    r->headers_out.status = NGX_HTTP_OK;  
    r->headers_out.content_length_n = content_length;  
    rc = ngx_http_send_header(r);  
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {  
        return rc;  
    }  
  
    return ngx_http_output_filter(r, &out);
}

static ngx_int_t ngx_http_index_gate_handler(ngx_http_request_t* r) {

    if(ngx_strncmp(r->uri.data, "/GPS-Server/position/car", 24) == 0){
        return update_car_position(r);
    } 
    else if (ngx_strncmp(r->uri.data, "/GPS-Server/status/car", 22) == 0){
        return update_car_status(r);
    }
    else if (ngx_strncmp(r->uri.data, "/GPS-Server/cars", 16) == 0){
        return post_car_positions(r);
    }
    
    return NGX_HTTP_NOT_ALLOWED;  
}  
  
static void* ngx_http_index_gate_create_loc_conf(ngx_conf_t* cf) {  
    ngx_http_index_gate_loc_conf_t* conf;  
  
    conf = (ngx_http_index_gate_loc_conf_t*)ngx_pcalloc(cf->pool, sizeof(ngx_http_index_gate_loc_conf_t));  
    if (conf == NULL) {  
        return NGX_CONF_ERROR;  
    }  
    conf->output_words.len = 0;  
    conf->output_words.data = NULL;  
  
    return conf;  
}  
  
static char* ngx_http_index_gate_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {  
    ngx_http_index_gate_loc_conf_t* prev = (ngx_http_index_gate_loc_conf_t*)parent;  
    ngx_http_index_gate_loc_conf_t* conf = (ngx_http_index_gate_loc_conf_t*)child;  
    ngx_conf_merge_str_value(conf->output_words, prev->output_words, "Nginx");  
    return NGX_CONF_OK;  
}  
  
static char* ngx_http_index_gate(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {  
    ngx_http_core_loc_conf_t* clcf;  
    clcf = (ngx_http_core_loc_conf_t*)ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);  
    clcf->handler = ngx_http_index_gate_handler;  
    ngx_conf_set_str_slot(cf, cmd, conf);  
    return NGX_CONF_OK;  
}  

static char* ngx_http_index_gate_cid(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {  
    ngx_http_index_gate_loc_conf_t* local_conf;
    local_conf = static_cast<ngx_http_index_gate_loc_conf_t*>(conf);

    ngx_str_t name = ngx_string("arg_cid");
    cid_index = ngx_http_get_variable_index(cf, &name);

    ngx_conf_set_str_slot(cf, cmd, conf);
    // ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "cid:%s", local_conf->cid.data);

    return NGX_CONF_OK;  
}  

static char* ngx_http_index_gate_lng(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {  
    ngx_http_index_gate_loc_conf_t* local_conf;
    local_conf = static_cast<ngx_http_index_gate_loc_conf_t*>(conf);

    ngx_str_t name = ngx_string("arg_lng");
    lng_index = ngx_http_get_variable_index(cf, &name);

    ngx_conf_set_str_slot(cf, cmd, conf);
    // ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "lng:%s", local_conf->lng.data);
    return NGX_CONF_OK;  
}  

static char* ngx_http_index_gate_lat(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    ngx_http_index_gate_loc_conf_t* local_conf;
    local_conf = static_cast<ngx_http_index_gate_loc_conf_t*>(conf);

    ngx_str_t name = ngx_string("arg_lat");
    lat_index = ngx_http_get_variable_index(cf, &name);

    ngx_conf_set_str_slot(cf, cmd, conf);
    // ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "lat:%s", local_conf->lat.data);
    return NGX_CONF_OK;  
}  

static char* ngx_http_index_gate_status(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    ngx_http_index_gate_loc_conf_t* local_conf;
    local_conf = static_cast<ngx_http_index_gate_loc_conf_t*>(conf);

    ngx_str_t name = ngx_string("arg_status");
    status_index = ngx_http_get_variable_index(cf, &name);

    ngx_conf_set_str_slot(cf, cmd, conf);
    // ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "status:%s", local_conf->lat.data);
    return NGX_CONF_OK;  
}  
