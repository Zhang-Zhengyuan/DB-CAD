#ifndef MGCLIENT_H
#define MGCLIENT_H

//若要启用SSL功能，请取消下面这行的注释，并在附加包含目录中添加C:\Program Files\OpenSSL-Win64\include;，对于Debug配置在附加依赖项中添加C:\Program Files\OpenSSL-Win64\lib\VC\libssl64MDd.lib;C:\Program Files\OpenSSL-Win64\lib\VC\libcrypto64MDd.lib;，对于Release配置在附加依赖项中添加C:\Program Files\OpenSSL-Win64\lib\VC\libssl64MD.lib;C:\Program Files\OpenSSL-Win64\lib\VC\libcrypto64MD.lib;
#define MGCLIENT_ENABLE_SSL

#define MGCLIENT_VERSION "1.4.1"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#ifdef MGCLIENT_ENABLE_SSL
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

#define alignof __alignof
typedef double max_align_t;

//mgclient.h-------------------------------------------------------------------------

#  define MGCLIENT_EXPORT
#  define MGCLIENT_NO_EXPORT
#  define MGCLIENT_DEPRECATED __declspec(deprecated)
#  define MGCLIENT_DEPRECATED_EXPORT MGCLIENT_EXPORT MGCLIENT_DEPRECATED
#  define MGCLIENT_DEPRECATED_NO_EXPORT MGCLIENT_NO_EXPORT MGCLIENT_DEPRECATED

/// Client software version.
///
/// \return Client version in the major.minor.patch format.
inline const char* mg_client_version();

/// Initializes the client (the whole process).
/// Should be called at the beginning of each process using the client.
///
/// \return Zero if initialization was successful.
inline int mg_init();

/// Finalizes the client (the whole process).
/// Should be called at the end of each process using the client.
inline void mg_finalize();

/// An enum listing all the types as specified by Bolt protocol.
enum mg_value_type {
    MG_VALUE_TYPE_NULL,
    MG_VALUE_TYPE_BOOL,
    MG_VALUE_TYPE_INTEGER,
    MG_VALUE_TYPE_FLOAT,
    MG_VALUE_TYPE_STRING,
    MG_VALUE_TYPE_LIST,
    MG_VALUE_TYPE_MAP,
    MG_VALUE_TYPE_NODE,
    MG_VALUE_TYPE_RELATIONSHIP,
    MG_VALUE_TYPE_UNBOUND_RELATIONSHIP,
    MG_VALUE_TYPE_PATH,
    MG_VALUE_TYPE_DATE,
    MG_VALUE_TYPE_TIME,
    MG_VALUE_TYPE_LOCAL_TIME,
    MG_VALUE_TYPE_DATE_TIME,
    MG_VALUE_TYPE_DATE_TIME_ZONE_ID,
    MG_VALUE_TYPE_LOCAL_DATE_TIME,
    MG_VALUE_TYPE_DURATION,
    MG_VALUE_TYPE_POINT_2D,
    MG_VALUE_TYPE_POINT_3D,
    MG_VALUE_TYPE_UNKNOWN
};

/// A Bolt value, encapsulating all other values.
typedef struct mg_value mg_value;

/// An UTF-8 encoded string.
///
/// Note that the length of the string is the byte count of the UTF-8 encoded
/// data. It is guaranteed that the bytes of the string are stored contiguously,
/// and they can be accessed through a pointer to first element returned by
/// \ref mg_string_data.
///
/// Note that the library doesn't perform any checks whatsoever to see if the
/// provided data is a valid UTF-8 encoded string when constructing instances of
/// \ref mg_string.
///
/// Maximum possible string length allowed by Bolt protocol is \c UINT32_MAX.
typedef struct mg_string mg_string;

/// An ordered sequence of values.
///
/// List may contain a mixture of different types as its elements. A list owns
/// all values stored in it.
///
/// Maximum possible list length allowed by Bolt is \c UINT32_MAX.
typedef struct mg_list mg_list;

/// Sized sequence of pairs of keys and values.
///
/// Map may contain a mixture of different types as values. A map owns all keys
/// and values stored in it.
///
/// Maximum possible map size allowed by Bolt protocol is \c UINT32_MAX.
typedef struct mg_map mg_map;

/// Represents a node from a labeled property graph.
///
/// Consists of a unique identifier (withing the scope of its origin graph), a
/// list of labels and a map of properties. A node owns its labels and
/// properties.
///
/// Maximum possible number of labels allowed by Bolt protocol is \c UINT32_MAX.
typedef struct mg_node mg_node;

/// Represents a relationship from a labeled property graph.
///
/// Consists of a unique identifier (within the scope of its origin graph),
/// identifiers for the start and end nodes of that relationship, a type and a
/// map of properties. A relationship owns its type string and property map.
typedef struct mg_relationship mg_relationship;

/// Represents a relationship from a labeled property graph.
///
/// Like \ref mg_relationship, but without identifiers for start and end nodes.
/// Mainly used as a supporting type for \ref mg_path. An unbound relationship
/// owns its type string and property map.
typedef struct mg_unbound_relationship mg_unbound_relationship;

/// Represents a sequence of alternating nodes and relationships
/// corresponding to a walk in a labeled property graph.
///
/// A path of length L consists of L + 1 nodes indexed from 0 to L, and L
/// unbound relationships, indexed from 0 to L - 1. Each relationship has a
/// direction. A relationship is said to be reversed if it was traversed in the
/// direction opposite of the direction of the underlying relationship in the
/// data graph.
typedef struct mg_path mg_path;

/// \brief Represents a date.
///
/// Date is defined with number of days since the Unix epoch.
typedef struct mg_date mg_date;

/// \brief Represents time with its time zone.
///
/// Time is defined with nanoseconds since midnight.
/// Timezone is defined with seconds from UTC.
typedef struct mg_time mg_time;

/// \brief Represents local time.
///
/// Time is defined with nanoseconds since midnight.
typedef struct mg_local_time mg_local_time;

/// \brief Represents date and time with its time zone.
///
/// Date is defined with seconds since the adjusted Unix epoch.
/// Time is defined with nanoseconds since midnight.
/// Time zone is defined with minutes from UTC.
typedef struct mg_date_time mg_date_time;

/// \brief Represents date and time with its time zone.
///
/// Date is defined with seconds since the adjusted Unix epoch.
/// Time is defined with nanoseconds since midnight.
/// Timezone is defined with an identifier for a specific time zone.
typedef struct mg_date_time_zone_id mg_date_time_zone_id;

/// \brief Represents date and time without its time zone.
///
/// Date is defined with seconds since the Unix epoch.
/// Time is defined with nanoseconds since midnight.
typedef struct mg_local_date_time mg_local_date_time;

/// \brief Represents a temporal amount which captures the difference in time
/// between two instants.
///
/// Duration is defined with months, days, seconds, and nanoseconds.
/// \note
/// Duration can be negative.
typedef struct mg_duration mg_duration;

/// \brief Represents a single location in 2-dimensional space.
///
/// Contains SRID along with its x and y coordinates.
typedef struct mg_point_2d mg_point_2d;

/// \brief Represents a single location in 3-dimensional space.
///
/// Contains SRID along with its x, y and z coordinates.
typedef struct mg_point_3d mg_point_3d;

/// Constructs a nil \ref mg_value.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_null();

/// Constructs a boolean \ref mg_value.
///
/// \param val If the parameter is zero, constructed value will be false.
///            Otherwise, it will be true.
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_bool(int val);

/// Constructs an integer \ref mg_value with the given underlying value.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_integer(int64_t val);

/// Constructs a float \ref mg_value with the given underlying value.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_float(double val);

/// Constructs a string \ref mg_value given a null-terminated string.
///
/// A new \ref mg_string instance will be created from the null-terminated
/// string as the underlying value.
///
/// \param str A null-terminated UTF-8 string.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_string(const char* str);

/// Construct a string \ref mg_value given the underlying \ref mg_string.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_string2(mg_string* str);

/// Constructs a list \ref mg_value given the underlying \ref mg_list.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_list(mg_list* list);

/// Constructs a map \ref mg_value given the underlying \ref mg_map.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_map(mg_map* map);

/// Constructs a node \ref mg_value given the underlying \ref mg_node.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_node(mg_node* node);

/// Constructs a relationship \ref mg_value given the underlying
/// \ref mg_relationship.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_relationship(mg_relationship* rel);

/// Constructs an unbound relationship \ref mg_value given the underlying
/// \ref mg_unbound_relationship.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_unbound_relationship(
    mg_unbound_relationship* rel);

/// Constructs a path \ref mg_value given the underlying \ref mg_path.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_path(mg_path* path);

/// Constructs a date \ref mg_value given the underlying \ref mg_date.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_date(mg_date* date);

/// Constructs a time \ref mg_value given the underlying \ref mg_time.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_time(mg_time* time);

/// Constructs a local time \ref mg_value given the underlying \ref
/// mg_local_time.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_local_time(mg_local_time* local_time);

/// Constructs a date and time \ref mg_value given the underlying \ref
/// mg_date_time.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_date_time(mg_date_time* date_time);

/// Constructs a date and time \ref mg_value given the underlying \ref
/// mg_date_time_zone_id.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_date_time_zone_id(
    mg_date_time_zone_id* date_time_zone_id);

/// Constructs a local date and time \ref mg_value given the underlying \ref
/// mg_local_date_time.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_local_date_time(
    mg_local_date_time* local_date_time);

/// Constructs a duration \ref mg_value given the underlying \ref mg_duration.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_duration(mg_duration* duration);

/// Constructs a 2D point \ref mg_value given the underlying \ref mg_point_2d.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_point_2d(mg_point_2d* point_2d);

/// Constructs a 3D point \ref mg_value given the underlying \ref mg_point_3d.
///
/// \return Pointer to the newly constructed value or NULL if error occurred.
inline mg_value* mg_value_make_point_3d(mg_point_3d* point_3d);

/// Returns the type of the given \ref mg_value.
inline enum mg_value_type mg_value_get_type(const mg_value* val);

/// Returns non-zero value if value contains true, zero otherwise.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline int mg_value_bool(const mg_value* val);

/// Returns the underlying integer value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline int64_t mg_value_integer(const mg_value* val);

/// Returns the underlying float value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline double mg_value_float(const mg_value* val);

/// Returns the underlying \ref mg_string value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_string* mg_value_string(const mg_value* val);

/// Returns the underlying \ref mg_list value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_list* mg_value_list(const mg_value* val);

/// Returns the underlying \ref mg_map value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_map* mg_value_map(const mg_value* val);

/// Returns the underlying \ref mg_node value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_node* mg_value_node(const mg_value* val);

/// Returns the underlying \ref mg_relationship value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_relationship* mg_value_relationship(
    const mg_value* val);

/// Returns the underlying \ref mg_unbound_relationship value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_unbound_relationship* mg_value_unbound_relationship(
    const mg_value* val);

/// Returns the underlying \ref mg_path value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_path* mg_value_path(const mg_value* val);

/// Returns the underlying \ref mg_date value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_date* mg_value_date(const mg_value* val);

/// Returns the underlying \ref mg_time value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_time* mg_value_time(const mg_value* val);

/// Returns the underlying \ref mg_local_time value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_local_time* mg_value_local_time(const mg_value* val);

/// Returns the underlying \ref mg_date_time value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_date_time* mg_value_date_time(const mg_value* val);

/// Returns the underlying \ref mg_date_time_zone_id value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_date_time_zone_id* mg_value_date_time_zone_id(
    const mg_value* val);

/// Returns the underlying \ref mg_local_date_time value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_local_date_time* mg_value_local_date_time(
    const mg_value* val);

/// Returns the underlying \ref mg_duration value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_duration* mg_value_duration(const mg_value* val);

/// Returns the underlying \ref mg_point_2d value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_point_2d* mg_value_point_2d(const mg_value* val);

/// Returns the underlying \ref mg_point_3d value.
///
/// Type check should be made first. Accessing the wrong value results in
/// undefined behavior.
inline const mg_point_3d* mg_value_point_3d(const mg_value* val);

/// Creates a copy of the given value.
///
/// \return Pointer to the copy or NULL if error occurred.
inline mg_value* mg_value_copy(const mg_value* val);

/// Destroys the given value.
inline void mg_value_destroy(mg_value* val);

/// Constructs a string given a null-terminated string.
///
/// A new buffer of appropriate length will be allocated and the given string
/// will be copied there.
///
/// \param str A null-terminated UTF-8 string.
///
/// \return A pointer to the newly constructed `mg_string` object or \c NULL
///         if an error occurred.
inline mg_string* mg_string_make(const char* str);

/// Constructs a string given its length (in bytes) and contents.
///
/// A new buffer of will be allocated and the given data will be copied there.
///
/// \param len  Number of bytes in the data buffer.
/// \param data The string contents.
///
/// \return A pointer to the newly constructed `mg_string` object or \c NULL
///         if an error occurred.
inline mg_string* mg_string_make2(uint32_t len, const char* data);

/// Returns a pointer to the beginning of data buffer of string \p str.
inline const char* mg_string_data(const mg_string* str);

/// Returns the length (in bytes) of string \p str.
inline uint32_t mg_string_size(const mg_string* str);

/// Creates a copy of the given string.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_string* mg_string_copy(const mg_string* str);

/// Destroys the given string.
inline void mg_string_destroy(mg_string* str);

/// Constructs a list that can hold at most \p capacity elements.
///
/// Elements should be constructed and then inserted using \ref mg_list_append.
///
/// \param capacity The maximum number of elements that the newly constructed
///                 list can hold.
///
/// \return A pointer to the newly constructed empty list or NULL if an error
///         occurred.
inline mg_list* mg_list_make_empty(uint32_t capacity);

/// Appends an element at the end of the list \p list.
///
/// Insertion will fail if the list capacity is already exhausted. If the
/// insertion fails, the map doesn't take ownership of \p value.
///
/// \param list  The list instance to be modified.
/// \param value The value to be appended.
///
/// \return The function returns non-zero value if insertion failed, zero
///         otherwise.
inline int mg_list_append(mg_list* list, mg_value* value);

/// Returns the number of elements in list \p list.
inline uint32_t mg_list_size(const mg_list* list);

/// Retrieves the element at position \p pos in list \p list.
///
/// \return A pointer to required list element. If \p pos is outside of list
///         bounds, \c NULL is returned.
inline const mg_value* mg_list_at(const mg_list* list, uint32_t pos);

/// Creates a copy of the given list.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_list* mg_list_copy(const mg_list* list);

/// Destroys the given list.
inline void mg_list_destroy(mg_list* list);

/// Constructs an empty map that can hold at most \p capacity key-value pairs.
///
/// Key-value pairs should be constructed and then inserted using
/// \ref mg_map_insert, \ref mg_map_insert_unsafe and similar.
///
/// \param capacity The maximum number of key-value pairs that the newly
///                 constructed list can hold.
///
/// \return A pointer to the newly constructed empty map or NULL if an error
///         occurred.
inline mg_map* mg_map_make_empty(uint32_t capacity);

/// Inserts the given key-value pair into the map.
///
/// A check is performed to see if the given key is unique in the map which
/// means that a number of key comparisons equal to the current number of
/// elements in the map is made.
///
/// If key length is greater that \c UINT32_MAX, or the key already exists in
/// map, or the map's capacity is exhausted, the insertion will fail. If
/// insertion fails, the map doesn't take ownership of \p value.
///
/// If the insertion is successful, a new \ref mg_string is constructed for
/// the storage of the key and the map takes ownership of \p value.
///
/// \param map     The map instance to be modifed.
/// \param key_str A null-terminated string to be used as key.
/// \param value   Value to be inserted.
///
/// \return The function returns non-zero value if insertion failed, zero
///         otherwise.
inline int mg_map_insert(mg_map* map, const char* key_str,
    mg_value* value);

/// Inserts the given key-value pair into the map.
///
/// A check is performed to see if the given key is unique in the map which
/// means that a number of key comparisons equal to the current number of
/// elements in the map is made.
///
/// If the key already exists in map, or the map's capacity is exhausted, the
/// insertion will fail. If insertion fails, the map doesn't take ownership of
/// \p key and \p value.
///
/// If the insertion is successful, map takes ownership of \p key and \p value.
///
/// \param map     The map instance to be modifed.
/// \param key     A \ref mg_string to be used as key.
/// \param value   Value to be inserted.
///
/// \return The function returns non-zero value if insertion failed, zero
///         otherwise.
inline int mg_map_insert2(mg_map* map, mg_string* key,
    mg_value* value);

/// Inserts the given key-value pair into the map.
///
/// No check is performed for key uniqueness. Note that map containing duplicate
/// keys is considered invalid in Bolt protocol.
///
/// If key length is greated than \c UINT32_MAX or or the map's capacity is
/// exhausted, the insertion will fail. If insertion fails, the map doesn't take
/// ownership of \p value.
///
/// If the insertion is successful, a new \ref mg_string is constructed for the
/// storage of the key and the map takes ownership of \p value.
///
/// \param map     The map instance to be modifed.
/// \param key_str A null-terminated string to be used as key.
/// \param value   Value to be inserted.
///
/// \return The function returns non-zero value if insertion failed, zero
///         otherwise.
inline int mg_map_insert_unsafe(mg_map* map, const char* key_str,
    mg_value* value);

/// Inserts the given key-value pair into the map.
///
/// No check is performed for key uniqueness. Note that map containing duplicate
/// keys is considered invalid in Bolt protocol.
///
/// If the map's capacity is exhausted, the insertion will fail. If insertion
/// fails, the map doesn't take ownership of \p key and \p value.
///
/// If the insertion is successful, map takes ownership of \p key and \p value.
///
/// \param map     The map instance to be modifed.
/// \param key     A \ref mg_string to be used as key.
/// \param value   Value to be inserted.
///
/// \return The function returns non-zero value if insertion failed, zero
///         otherwise.
inline int mg_map_insert_unsafe2(mg_map* map, mg_string* key,
    mg_value* value);

/// Looks up a map value with the given key.
///
/// \param map     The map instance to be queried.
/// \param key_str A null-terminated string representing the key to be looked-up
///                in the map.
///
/// \return If the key is found in the map, the pointer to the corresponding
///         \ref mg_value is returned. Otherwise, \c NULL is returned.
inline const mg_value* mg_map_at(const mg_map* map,
    const char* key_str);

/// Looks up a map value with the given key.
///
/// \param map      The map instance to be queried.
/// \param key_size The length of the string representing the key to be
///                 looked-up in the map.
/// \param key_data Bytes constituting the key string.
///
/// \return If the key is found in the map, the pointer to the corresponding
///         \ref mg_value is returned. Otherwise, \c NULL is returned.
inline const mg_value* mg_map_at2(const mg_map* map, uint32_t key_size,
    const char* key_data);

/// Returns the number of key-value pairs in map \p map.
inline uint32_t mg_map_size(const mg_map* map);

/// Retrieves the key at position \p pos in map \p map.
///
/// \return A pointer to required key. If \p pos is outside of map bounds, \c
///         NULL is returned.
inline const mg_string* mg_map_key_at(const mg_map*, uint32_t pos);

/// Retrieves the value at position \p pos in map \p map.
///
/// \return A pointer to required value. If \p pos is outside of map bounds,
///         \c NULL is returned.
inline const mg_value* mg_map_value_at(const mg_map*, uint32_t pos);

/// Creates a copy of the given map.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_map* mg_map_copy(const mg_map* map);

/// Destroys the given map.
inline void mg_map_destroy(mg_map* map);

/// Returns the ID of node \p node.
inline int64_t mg_node_id(const mg_node* node);

/// Returns the number of labels of node \p node.
inline uint32_t mg_node_label_count(const mg_node* node);

/// Returns the label at position \p pos in node \p node's label list.
///
/// \return A pointer to the required label. If \p pos is outside of label list
///         bounds, \c NULL is returned.
inline const mg_string* mg_node_label_at(const mg_node* node,
    uint32_t pos);

/// Returns property map of node \p node.
inline const mg_map* mg_node_properties(const mg_node* node);

/// Creates a copy of the given node.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_node* mg_node_copy(const mg_node* node);

/// Destroys the given node.
inline void mg_node_destroy(mg_node* node);

/// Returns the ID of the relationship \p rel.
inline int64_t mg_relationship_id(const mg_relationship* rel);

/// Returns the ID of the start node of relationship \p rel.
inline int64_t mg_relationship_start_id(const mg_relationship* rel);

/// Returns the ID of the end node of relationship \p rel.
inline int64_t mg_relationship_end_id(const mg_relationship* rel);

/// Returns the type of the relationship \p rel.
inline const mg_string* mg_relationship_type(
    const mg_relationship* rel);

/// Returns the property map of the relationship \p rel.
inline const mg_map* mg_relationship_properties(
    const mg_relationship* rel);

/// Creates a copy of the given relationship.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_relationship* mg_relationship_copy(
    const mg_relationship* rel);

/// Destroys the given relationship.
inline void mg_relationship_destroy(mg_relationship* rel);

/// Returns the ID of the unbound relationship \p rel.
inline int64_t
mg_unbound_relationship_id(const mg_unbound_relationship* rel);

/// Returns the type of the unbound relationship \p rel.
inline const mg_string* mg_unbound_relationship_type(
    const mg_unbound_relationship* rel);

/// Returns the property map of the unbound relationship \p rel.
inline const mg_map* mg_unbound_relationship_properties(
    const mg_unbound_relationship* rel);

/// Creates a copy of the given unbound relationship.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_unbound_relationship* mg_unbound_relationship_copy(
    const mg_unbound_relationship* rel);

/// Destroys the given unbound relationship.
inline void mg_unbound_relationship_destroy(
    mg_unbound_relationship* rel);

/// Returns the length (the number of edges) of path \p path.
inline uint32_t mg_path_length(const mg_path* path);

/// Returns the node at position \p pos in the traversal of path \p path.
///
/// Nodes are indexed from 0 to path length.
///
/// \return A pointer to the required node. If \p pos is out of path bounds, \c
///         NULL is returned.
inline const mg_node* mg_path_node_at(const mg_path* path,
    uint32_t pos);

/// Returns the relationship at position \p pos in traversal of path \p path.
///
/// Relationships are indexed from 0 to path length - 1.
///
/// \return A pointer to the required relationship. If \p pos is outside of
///         path bounds, \c NULL is returned.
inline const mg_unbound_relationship* mg_path_relationship_at(
    const mg_path* path, uint32_t pos);

/// Checks if the relationship at position \p pos in traversal of path \p path
/// is reversed.
///
/// Relationships are indexed from 0 to path length - 1.
///
/// \return Returns 0 if relationships is traversed in the same direction as the
///         underlying relationship in the data graph, and 1 if it is traversed
///         in the opposite direction. If \p pos is outside of path bounds, -1
///         is returned.
inline int mg_path_relationship_reversed_at(const mg_path* path,
    uint32_t pos);

/// Creates a copy of the given path.
///
/// \return A pointer to the copy or NULL if an error occurred.
inline mg_path* mg_path_copy(const mg_path* path);

/// Destroys the given path.
inline void mg_path_destroy(mg_path* path);

/// Creates mg_date from days.
/// \return a pointer to mg_date or NULL if an error occurred.
inline mg_date* mg_date_make(int64_t days);

/// Returns days since the Unix epoch.
inline int64_t mg_date_days(const mg_date* date);

/// Creates a copy of the given date.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_date* mg_date_copy(const mg_date* date);

/// Destroys the given date.
inline void mg_date_destroy(mg_date* date);

/// Returns nanoseconds since midnight.
inline int64_t mg_time_nanoseconds(const mg_time* time);

/// Returns time zone offset in seconds from UTC.
inline int64_t mg_time_tz_offset_seconds(const mg_time* time);

/// Creates a copy of the given time.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_time* mg_time_copy(const mg_time* time);

/// Destroys the given time.
inline void mg_time_destroy(mg_time* time);

/// Returns nanoseconds since midnight.
inline int64_t
mg_local_time_nanoseconds(const mg_local_time* local_time);

/// Creates mg_local_time from nanoseconds.
/// \return a pointer to mg_local_time or NULL if an error occurred.
inline mg_local_time* mg_local_time_make(int64_t nanoseconds);

/// Creates a copy of the given local time.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_local_time* mg_local_time_copy(
    const mg_local_time* local_time);

/// Destroys the given local time.
inline void mg_local_time_destroy(mg_local_time* local_time);

/// Returns seconds since Unix epoch.
inline int64_t mg_date_time_seconds(const mg_date_time* date_time);

/// Returns nanoseconds since midnight.
inline int64_t mg_date_time_nanoseconds(const mg_date_time* date_time);

/// Returns time zone offset in minutes from UTC.
inline int64_t
mg_date_time_tz_offset_minutes(const mg_date_time* date_time);

/// Creates a copy of the given date and time.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_date_time* mg_date_time_copy(const mg_date_time* date_time);

/// Destroys the given date and time.
inline void mg_date_time_destroy(mg_date_time* date_time);

/// Returns seconds since Unix epoch.
inline int64_t
mg_date_time_zone_id_seconds(const mg_date_time_zone_id* date_time_zone_id);

/// Returns nanoseconds since midnight.
inline int64_t
mg_date_time_zone_id_nanoseconds(const mg_date_time_zone_id* date_time_zone_id);

/// Returns time zone represented by the identifier.
inline int64_t
mg_date_time_zone_id_tz_id(const mg_date_time_zone_id* date_time_zone_id);

/// Creates a copy of the given date and time.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_date_time_zone_id* mg_date_time_zone_id_copy(
    const mg_date_time_zone_id* date_time_zone_id);

/// Destroys the given date and time.
inline void mg_date_time_zone_id_destroy(
    mg_date_time_zone_id* date_time_zone_id);

/// Creates mg_local_date_time from seconds and nanoseconds.
/// \return a pointer to mg_local_date_time or NULL if an error occurred.
inline mg_local_date_time* mg_local_date_time_make(
    int64_t seconds, int64_t nanoseconds);
//
/// Returns seconds since Unix epoch. This includes the hours, minutes, seconds
/// fields of the local_time.
inline int64_t
mg_local_date_time_seconds(const mg_local_date_time* local_date_time);

/// Returns subseconds of the local_time field as nanoseconds.
inline int64_t
mg_local_date_time_nanoseconds(const mg_local_date_time* local_date_time);

/// Creates a copy of the given local date and time.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_local_date_time* mg_local_date_time_copy(
    const mg_local_date_time* local_date_time);

/// Destroy the given local date and time.
inline void mg_local_date_time_destroy(
    mg_local_date_time* local_date_time);

/// Creates mg_duration from months, days, seconds and nanoseconds.
/// \return a pointer to mg_duration or NULL if an error occurred.
inline mg_duration* mg_duration_make(int64_t months, int64_t days,
    int64_t seconds,
    int64_t nanoseconds);

/// Returns the months part of the temporal amount.
inline int64_t mg_duration_months(const mg_duration* duration);

/// Returns the days part of the temporal amount.
inline int64_t mg_duration_days(const mg_duration* duration);

/// Returns the seconds part of the temporal amount.
inline int64_t mg_duration_seconds(const mg_duration* duration);

/// Returns the nanoseconds part of the temporal amount.
inline int64_t mg_duration_nanoseconds(const mg_duration* duration);

/// Creates a copy of the given duration.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_duration* mg_duration_copy(const mg_duration* duration);

/// Destroy the given duration.
inline void mg_duration_destroy(mg_duration* duration);

/// Returns SRID of the 2D point.
inline int64_t mg_point_2d_srid(const mg_point_2d* point_2d);

/// Returns the x coordinate of the 2D point.
inline double mg_point_2d_x(const mg_point_2d* point_2d);

/// Returns the y coordinate of the 2D point.
inline double mg_point_2d_y(const mg_point_2d* point_2d);

/// Creates a copy of the given 2D point.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_point_2d* mg_point_2d_copy(const mg_point_2d* point_2d);

/// Destroys the given 2D point.
inline void mg_point_2d_destroy(mg_point_2d* point_2d);

/// Returns SRID of the 3D point.
inline int64_t mg_point_3d_srid(const mg_point_3d* point_3d);

/// Returns the x coordinate of the 3D point.
inline double mg_point_3d_x(const mg_point_3d* point_3d);

/// Returns the y coordinate of the 3D point.
inline double mg_point_3d_y(const mg_point_3d* point_3d);

/// Returns the z coordinate of the 3D point.
inline double mg_point_3d_z(const mg_point_3d* point_3d);

/// Creates a copy of the given 3D point.
///
/// \return A pointer to the copy or NULL if an error occured.
inline mg_point_3d* mg_point_3d_copy(const mg_point_3d* point_3d);

/// Destroys the given 3D point.
inline void mg_point_3d_destroy(mg_point_3d* point_3d);

/// Marks a \ref mg_session ready to execute a new query using \ref
/// mg_session_run.
#define MG_SESSION_READY 0

/// Marks a \ref mg_session which is currently executing a query. Results can be
/// pulled using \ref mg_session_pull.
#define MG_SESSION_EXECUTING 1

/// Marks a bad \ref mg_session which cannot be used to execute queries and can
/// only be destroyed.
#define MG_SESSION_BAD 2

/// Marks a \ref mg_session which is currently fetching result of a query.
/// Results can be fetched using \ref mg_session_fetch.
#define MG_SESSION_FETCHING 3

/// Success code.
#define MG_SUCCESS (0)

/// Failed to send data to server.
#define MG_ERROR_SEND_FAILED (-1)

/// Failed to receive data from server.
#define MG_ERROR_RECV_FAILED (-2)

/// Out of memory.
#define MG_ERROR_OOM (-3)

/// Trying to insert more values in a full container.
#define MG_ERROR_CONTAINER_FULL (-4)

/// Invalid value type was given as a function argument.
#define MG_ERROR_INVALID_VALUE (-5)

/// Failed to decode data returned from server.
#define MG_ERROR_DECODING_FAILED (-6)

/// Trying to insert a duplicate key in map.
#define MG_ERROR_DUPLICATE_KEY (-7)

/// An error occurred while trying to connect to server.
#define MG_ERROR_NETWORK_FAILURE (-8)

/// Invalid parameter supplied to \ref mg_connect.
#define MG_ERROR_BAD_PARAMETER (-9)

/// Server violated the Bolt protocol by sending an invalid message type or
/// invalid value.
#define MG_ERROR_PROTOCOL_VIOLATION (-10)

/// Server sent a FAILURE message containing ClientError code.
#define MG_ERROR_CLIENT_ERROR (-11)

/// Server sent a FAILURE message containing TransientError code.
#define MG_ERROR_TRANSIENT_ERROR (-12)

/// Server sent a FAILURE message containing DatabaseError code.
#define MG_ERROR_DATABASE_ERROR (-13)

/// Got an unknown error message from server.
#define MG_ERROR_UNKNOWN_ERROR (-14)

/// Invalid usage of the library.
#define MG_ERROR_BAD_CALL (-15)

/// Maximum container size allowed by Bolt exceeded.
#define MG_ERROR_SIZE_EXCEEDED (-16)

/// An error occurred during SSL connection negotiation.
#define MG_ERROR_SSL_ERROR (-17)

/// User provided trust callback returned a non-zeron value after SSL connection
/// negotiation.
#define MG_ERROR_TRUST_CALLBACK (-18)

// Unable to initialize the socket (both create and connect).
#define MG_ERROR_SOCKET (-100)

// Function unimplemented.
#define MG_ERROR_UNIMPLEMENTED (-1000)

/// Determines whether a secure SSL TCP/IP connection will be negotiated with
/// the server.
enum mg_sslmode {
    MG_SSLMODE_DISABLE,  ///< Only try a non-SSL connection.
#ifndef __EMSCRIPTEN__
    MG_SSLMODE_REQUIRE,  ///< Only try a SSL connection.
#endif
};

/// An object encapsulating a Bolt session.
typedef struct mg_session mg_session;

/// An object containing parameters for `mg_connect`.
///
/// Currently recognized parameters are:
///  - host
///
///      DNS resolvable name of host to connect to. Exactly one of host and
///      address parameters must be specified.
///
///  - address
///
///      Numeric IP address of host to connect to. This should be in the
///      standard IPv4 address format. You can also use IPv6 if your machine
///      supports it. Exactly one of host and address parameters must be
///      specified.
///
///  - port
///
///      Port number to connect to at the server host.
///
///  - username
///
///      Username to connect as.
///
///  - password
///
///      Password to be used if the server demands password authentication.
///
///  - user_agent
///
///      Alternate name and version of the client to send to server. Default is
///      "MemgraphBolt/0.1".
///
///  - sslmode
///
///      This option determines whether a secure connection will be negotiated
///      with the server. There are 2 possible values:
///
///      - \ref MG_SSLMODE_DISABLE
///
///        Only try a non-SSL connection (default).
///
///      - \ref MG_SSLMODE_REQUIRE
///
///        Only try an SSL connection.
///
///  - sslcert
///
///      This parameter specifies the file name of the client SSL certificate.
///      It is ignored in case an SSL connection is not made.
///
///  - sslkey
///
///     This parameter specifies the location of the secret key used for the
///     client certificate. This parameter is ignored in case an SSL connection
///     is not made.
///
///  - trust_callback
///
///     A pointer to a function of prototype:
///        int trust_callback(const char *hostname, const char *ip_address,
///                           const char *key_type, const char *fingerprint,
///                           void *trust_data);
///
///     After performing the SSL handshake, \ref mg_connect will call this
///     function providing the hostname, IP address, public key type and
///     fingerprint and user provided data. If the function returns a non-zero
///     value, SSL connection will be immediately terminated. This can be used
///     to implement TOFU (trust on first use) mechanism.
///     It might happen that hostname can not be determined, in that case the
///     trust callback will be called with hostname="undefined".
///
///  - trust_data
///
///    Additional data that will be provided to trust_callback function.
typedef struct mg_session_params mg_session_params;

/// Prototype of the callback function for verifying an SSL connection by user.
typedef int (*mg_trust_callback_type)(const char*, const char*, const char*,
    const char*, void*);

/// Creates a new `mg_session_params` object.
inline mg_session_params* mg_session_params_make();

/// Destroys a `mg_session_params` object.
inline void mg_session_params_destroy(mg_session_params*);

/// Getters and setters for `mg_session_params` values.
inline void mg_session_params_set_address(mg_session_params*,
    const char* address);
inline void mg_session_params_set_host(mg_session_params*,
    const char* host);
inline void mg_session_params_set_port(mg_session_params*,
    uint16_t port);
inline void mg_session_params_set_username(mg_session_params*,
    const char* username);
inline void mg_session_params_set_password(mg_session_params*,
    const char* password);
inline void mg_session_params_set_user_agent(mg_session_params*,
    const char* user_agent);
inline void mg_session_params_set_sslmode(mg_session_params*,
    enum mg_sslmode sslmode);
inline void mg_session_params_set_sslcert(mg_session_params*,
    const char* sslcert);
inline void mg_session_params_set_sslkey(mg_session_params*,
    const char* sslkey);
inline void mg_session_params_set_trust_callback(
    mg_session_params*, mg_trust_callback_type trust_callback);
inline void mg_session_params_set_trust_data(mg_session_params*,
    void* trust_data);

inline const char* mg_session_params_get_address(
    const mg_session_params*);
inline const char* mg_session_params_get_host(
    const mg_session_params*);
inline uint16_t mg_session_params_get_port(const mg_session_params*);
inline const char* mg_session_params_get_username(
    const mg_session_params*);
inline const char* mg_session_params_get_password(
    const mg_session_params*);
inline const char* mg_session_params_get_user_agent(
    const mg_session_params*);
inline enum mg_sslmode mg_session_params_get_sslmode(
    const mg_session_params*);
inline const char* mg_session_params_get_sslcert(
    const mg_session_params*);
inline const char* mg_session_params_get_sslkey(
    const mg_session_params*);
inline mg_trust_callback_type
mg_session_params_get_trust_callback(const mg_session_params* params);
inline void* mg_session_params_get_trust_data(
    const mg_session_params*);

/// Makes a new connection to the database server.
///
/// This function opens a new database connection using the parameters specified
/// in provided \p params argument.
///
/// \param[in]  params  New Bolt connection parameters. See documentation for
///                     \ref mg_session_params.
/// \param[out] session A pointer to a newly created \ref mg_session is written
///                     here, unless there wasn't enough memory to allocate a
///                     \ref mg_session object. In that case, it is set to NULL.
///
/// \return Returns 0 if connected successfuly, otherwise returns a non-zero
///         error code. A more detailed error message can be obtained by using
///         \ref mg_session_error on \p session, unless it is set to NULL.
inline int mg_connect(const mg_session_params* params,
    mg_session** session);

/// Returns the status of \ref mg_session.
///
/// \return One of \ref MG_SESSION_READY, \ref MG_SESSION_EXECUTING,
/// \ref MG_SESSION_BAD.
inline int mg_session_status(const mg_session* session);

/// Obtains the error message stored in \ref mg_session (if any).
inline const char* mg_session_error(mg_session* session);

/// Destroys a \ref mg_session and releases all of its resources.
inline void mg_session_destroy(mg_session* session);

/// An object encapsulating a single result row or query execution summary. Its
/// lifetime is limited by lifetime of parent \ref mg_session. Also, invoking
/// \ref mg_session_pull ends the lifetime of previously returned \ref
/// mg_result.
typedef struct mg_result mg_result;

/// Submits a query to the server for execution.
///
/// All records from the previous query must be pulled before executing the
/// next query.
///
/// \param session               A \ref mg_session to be used for query
///                              execution.
/// \param query                 Query string.
/// \param params                A \ref mg_map containing query parameters. NULL
///                              can be supplied instead of an empty parameter
///                              map.
/// \param columns               Names of the columns output by the query
///                              execution will be stored in here. This is the
///                              same as the value
///                              obtained by \ref mg_result_columns on a pulled
///                              \ref mg_result. NULL can be supplied if we're
///                              not interested in the columns names.
/// \param extra_run_information A \ref mg_map containing extra information for
///                              running the statement.
///                              It can contain the following information:
///                               - bookmarks - list of strings containing some
///                               kind of bookmark identification
///                               - tx_timeout - integer that specifies a
///                               transaction timeout in ms.
///                               - tx_metadata - dictionary taht can contain
///                               some metadata information, mainly used for
///                               logging.
///                               - mode - specifies what kind of server is the
///                               run targeting. For write access use "w" and
///                               for read access use "r". Defaults to write
///                               access.
///                               - db - specifies the database name for
///                               multi-database to select where the transaction
///                               takes place. If no `db` is sent or empty
///                               string it implies that it is the default
///                               database.
/// \param qid                    QID for the statement will be stored in here
///                               if an Explicit transaction was started.
/// \return Returns 0 if query was submitted for execution successfuly.
///         Otherwise, a non-zero error code is returned.
inline int mg_session_run(mg_session* session, const char* query,
    const mg_map* params,
    const mg_map* extra_run_information,
    const mg_list** columns, int64_t* qid);

/// Starts an Explicit transaction on the server.
///
/// Every run will be part of that transaction until its explicitly ended.
///
/// \param session               A \ref mg_session on which the transaction
/// should be started. \param extra_run_information A \ref mg_map containing
/// extra information that will be used for every statement that is ran as part
/// of the transaction.
///                              It can contain the following information:
///                               - bookmarks - list of strings containing some
///                               kind of bookmark identification
///                               - tx_timeout - integer that specifies a
///                               transaction timeout in ms.
///                               - tx_metadata - dictionary taht can contain
///                               some metadata information, mainly used for
///                               logging.
///                               - mode - specifies what kind of server is the
///                               run targeting. For write access use "w" and
///                               for read access use "r". Defaults to write
///                               access.
///                               - db - specifies the database name for
///                               multi-database to select where the transaction
///                               takes place. If no `db` is sent or empty
///                               string it implies that it is the default
///                               database.
/// \return Returns 0 if the transaction was started successfuly.
///         Otherwise, a non-zero error code is returned.
inline int mg_session_begin_transaction(
    mg_session* session, const mg_map* extra_run_information);

/// Commits current Explicit transaction.
///
/// \param session A \ref mg_session on which the transaction should
///                be commited.
/// \param result  Contains the information about the commited transaction
///                if it was successful.
/// \return Returns 0 if the  transaction was ended successfuly.
///         Otherwise, a non-zero error code is returned.
inline int mg_session_commit_transaction(mg_session* session,
    mg_result** result);

/// Rollbacks current Explicit transaction.
///
/// \param session A \ref mg_session on which the transaction should
///                be rollbacked.
/// \param result  Contains the information about the rollbacked transaction
///                if it was successful.
/// \return Returns 0 if the transaction was ended successfuly.
///         Otherwise, a non-zero error code is returned.
inline int mg_session_rollback_transaction(mg_session* session,
    mg_result** result);

/// Tries to fetch the next query result from \ref mg_session.
///
/// The owner of the returned result is \ref mg_session \p session, and the
/// result is destroyed on next call to \ref mg_session_fetch.
///
/// \return On success, 0 or 1 is returned. Exit code 1 means that a new result
///         row was obtained and stored in \p result and its contents may be
///         accessed using \ref mg_result_row. Exit code 0 means that there are
///         no more result rows and that the query execution summary was stored
///         in \p result. Its contents may be accessed using \ref
///         mg_result_summary. On failure, a non-zero exit code is returned.
inline int mg_session_fetch(mg_session* session, mg_result** result);

/// Tries to pull results of a statement.
///
/// \param session          A \ref mg_session from which the results should be
///                         pulled.
/// \param pull_information A \ref mg_map that contains extra information for
///                         pulling the results.
///                         It can contain the following information:
///                          - n - how many records to fetch. `n=-1` will fetch
///                          all records.
///                          - qid - query identification, specifies the result
///                          from which statement the results should be pulled.
///                          `qid=-1` denotes the last executed statement. This
///                          is only for Explicit transactions.
/// \return Returns 0 if the result was pulled successfuly.
///         Otherwise, a non-zero error code is returned.
inline int mg_session_pull(mg_session* session,
    const mg_map* pull_information);

/// Returns names of columns output by the current query execution.
inline const mg_list* mg_result_columns(const mg_result* result);

/// Returns column values of current result row.
inline const mg_list* mg_result_row(const mg_result* result);

/// Returns query execution summary.
inline const mg_map* mg_result_summary(const mg_result* result);



//mgconstants.h-------------------------------------------------------------------------

#define MG_BOLT_CHUNK_HEADER_SIZE 2
#define MG_BOLT_MAX_CHUNK_SIZE 65535

#define MG_TINY_INT_MIN -16
#define MG_TINY_INT_MAX 127

#define MG_TINY_SIZE_MAX 15

inline static const char MG_HANDSHAKE_MAGIC[] = "\x60\x60\xB0\x17";
inline static const char MG_USER_AGENT[] = "mgclient/" MGCLIENT_VERSION;

/// Markers
#define MG_MARKER_NULL 0xC0
#define MG_MARKER_BOOL_FALSE 0xC2
#define MG_MARKER_BOOL_TRUE 0xC3

#define MG_MARKER_INT_8 0xC8
#define MG_MARKER_INT_16 0xC9
#define MG_MARKER_INT_32 0xCA
#define MG_MARKER_INT_64 0xCB

#define MG_MARKER_FLOAT 0xC1

#define MG_MARKER_TINY_STRING 0x80
#define MG_MARKER_STRING_8 0xD0
#define MG_MARKER_STRING_16 0xD1
#define MG_MARKER_STRING_32 0xD2

#define MG_MARKER_TINY_LIST 0x90
#define MG_MARKER_LIST_8 0xD4
#define MG_MARKER_LIST_16 0xD5
#define MG_MARKER_LIST_32 0xD6

#define MG_MARKER_TINY_MAP 0xA0
#define MG_MARKER_MAP_8 0xD8
#define MG_MARKER_MAP_16 0xD9
#define MG_MARKER_MAP_32 0xDA

// These have to be ordered from smallest to largest because
// `mg_session_write_container_size` and `mg_session_read_container_size` depend
// on that.
inline static const uint8_t MG_MARKERS_STRING[] = {
    MG_MARKER_TINY_STRING, MG_MARKER_STRING_8, MG_MARKER_STRING_16,
    MG_MARKER_STRING_32 };

inline static const uint8_t MG_MARKERS_LIST[] = { MG_MARKER_TINY_LIST, MG_MARKER_LIST_8,
                                          MG_MARKER_LIST_16, MG_MARKER_LIST_32 };

inline static const uint8_t MG_MARKERS_MAP[] = { MG_MARKER_TINY_MAP, MG_MARKER_MAP_8,
                                         MG_MARKER_MAP_16, MG_MARKER_MAP_32 };

#define MG_MARKER_TINY_STRUCT 0xB0
#define MG_MARKER_TINY_STRUCT1 0xB1
#define MG_MARKER_TINY_STRUCT2 0xB2
#define MG_MARKER_TINY_STRUCT3 0xB3
#define MG_MARKER_TINY_STRUCT4 0xB4
#define MG_MARKER_TINY_STRUCT5 0xB5
#define MG_MARKER_STRUCT_8 0xDC
#define MG_MARKER_STRUCT_16 0xDD

// Struct signatures
#define MG_SIGNATURE_NODE 0x4E
#define MG_SIGNATURE_RELATIONSHIP 0x52
#define MG_SIGNATURE_UNBOUND_RELATIONSHIP 0x72
#define MG_SIGNATURE_PATH 0x50
#define MG_SIGNATURE_DATE 0x44
#define MG_SIGNATURE_TIME 0x54
#define MG_SIGNATURE_LOCAL_TIME 0x74
#define MG_SIGNATURE_DATE_TIME 0x46
#define MG_SIGNATURE_DATE_TIME_ZONE_ID 0x66
#define MG_SIGNATURE_LOCAL_DATE_TIME 0x64
#define MG_SIGNATURE_DURATION 0x45
#define MG_SIGNATURE_POINT_2D 0x58
#define MG_SIGNATURE_POINT_3D 0x59
#define MG_SIGNATURE_MESSAGE_HELLO 0x01
#define MG_SIGNATURE_MESSAGE_RUN 0x10
#define MG_SIGNATURE_MESSAGE_PULL 0x3F
#define MG_SIGNATURE_MESSAGE_RECORD 0x71
#define MG_SIGNATURE_MESSAGE_SUCCESS 0x70
#define MG_SIGNATURE_MESSAGE_FAILURE 0x7F
#define MG_SIGNATURE_MESSAGE_ACK_FAILURE 0x0E
#define MG_SIGNATURE_MESSAGE_RESET 0x0F
#define MG_SIGNATURE_MESSAGE_BEGIN 0x11
#define MG_SIGNATURE_MESSAGE_COMMIT 0x12
#define MG_SIGNATURE_MESSAGE_ROLLBACK 0x13

//mgallocator.h-------------------------------------------------------------------------

typedef struct mg_allocator {
    void* (*malloc)(struct mg_allocator* self, size_t size);
    void* (*realloc)(struct mg_allocator* self, void* buf, size_t size);
    void (*free)(struct mg_allocator* self, void* buf);
} mg_allocator;

inline void* mg_allocator_malloc(struct mg_allocator* allocator, size_t size);

inline void* mg_allocator_realloc(struct mg_allocator* allocator, void* buf,
    size_t size);

inline void mg_allocator_free(struct mg_allocator* allocator, void* buf);

typedef struct mg_linear_allocator mg_linear_allocator;

inline mg_linear_allocator* mg_linear_allocator_init(mg_allocator* allocator,
    size_t block_size,
    size_t sep_alloc_threshold);

void mg_linear_allocator_reset(mg_linear_allocator* allocator);

void mg_linear_allocator_destroy(mg_linear_allocator* allocator);

//mgvalue.h-------------------------------------------------------------------------

typedef struct mg_value mg_value;

typedef struct mg_string {
    uint32_t size;
    char* data;
} mg_string;

typedef struct mg_list {
    uint32_t size;
    uint32_t capacity;
    mg_value** elements;
} mg_list;

typedef struct mg_map {
    uint32_t size;
    uint32_t capacity;
    mg_string** keys;
    mg_value** values;
} mg_map;

typedef struct mg_node {
    int64_t id;
    uint32_t label_count;
    mg_string** labels;
    mg_map* properties;
} mg_node;

typedef struct mg_relationship {
    int64_t id;
    int64_t start_id;
    int64_t end_id;
    mg_string* type;
    mg_map* properties;
} mg_relationship;

typedef struct mg_unbound_relationship {
    int64_t id;
    mg_string* type;
    mg_map* properties;
} mg_unbound_relationship;

typedef struct mg_path {
    uint32_t node_count;
    uint32_t relationship_count;
    uint32_t sequence_length;
    mg_node** nodes;
    mg_unbound_relationship** relationships;
    int64_t* sequence;
} mg_path;

typedef struct mg_date {
    int64_t days;
} mg_date;

typedef struct mg_time {
    int64_t nanoseconds;
    int64_t tz_offset_seconds;
} mg_time;

typedef struct mg_local_time {
    int64_t nanoseconds;
} mg_local_time;

typedef struct mg_date_time {
    int64_t seconds;
    int64_t nanoseconds;
    int64_t tz_offset_minutes;
} mg_date_time;

typedef struct mg_date_time_zone_id {
    int64_t seconds;
    int64_t nanoseconds;
    int64_t tz_id;
} mg_date_time_zone_id;

typedef struct mg_local_date_time {
    int64_t seconds;
    int64_t nanoseconds;
} mg_local_date_time;

typedef struct mg_duration {
    int64_t months;
    int64_t days;
    int64_t seconds;
    int64_t nanoseconds;
} mg_duration;

typedef struct mg_point_2d {
    int64_t srid;
    double x;
    double y;
} mg_point_2d;

typedef struct mg_point_3d {
    int64_t srid;
    double x;
    double y;
    double z;
} mg_point_3d;

struct mg_value {
    enum mg_value_type type;
    union {
        int bool_v;
        int64_t integer_v;
        double float_v;
        mg_string* string_v;
        mg_list* list_v;
        mg_map* map_v;
        mg_node* node_v;
        mg_relationship* relationship_v;
        mg_unbound_relationship* unbound_relationship_v;
        mg_path* path_v;
        mg_date* date_v;
        mg_time* time_v;
        mg_local_time* local_time_v;
        mg_date_time* date_time_v;
        mg_date_time_zone_id* date_time_zone_id_v;
        mg_local_date_time* local_date_time_v;
        mg_duration* duration_v;
        mg_point_2d* point_2d_v;
        mg_point_3d* point_3d_v;
    };
};

inline mg_string* mg_string_alloc(uint32_t size, mg_allocator* allocator);

inline mg_list* mg_list_alloc(uint32_t size, mg_allocator* allocator);

inline mg_map* mg_map_alloc(uint32_t size, mg_allocator* allocator);

inline mg_node* mg_node_alloc(uint32_t label_count, mg_allocator* allocator);

inline mg_path* mg_path_alloc(uint32_t node_count, uint32_t relationship_count,
    uint32_t sequence_length, mg_allocator* allocator);

inline mg_date* mg_date_alloc(mg_allocator* allocator);

inline mg_time* mg_time_alloc(mg_allocator* allocator);

inline mg_local_time* mg_local_time_alloc(mg_allocator* allocator);

inline mg_date_time* mg_date_time_alloc(mg_allocator* allocator);

inline mg_date_time_zone_id* mg_date_time_zone_id_alloc(mg_allocator* allocator);

inline mg_local_date_time* mg_local_date_time_alloc(mg_allocator* allocator);

inline mg_duration* mg_duration_alloc(mg_allocator* allocator);

inline mg_point_2d* mg_point_2d_alloc(mg_allocator* allocator);

inline mg_point_3d* mg_point_3d_alloc(mg_allocator* allocator);

inline mg_node* mg_node_make(int64_t id, uint32_t label_count, mg_string** labels,
    mg_map* properties);

inline mg_relationship* mg_relationship_make(int64_t id, int64_t start_id,
    int64_t end_id, mg_string* type,
    mg_map* properties);

inline mg_unbound_relationship* mg_unbound_relationship_make(int64_t id,
    mg_string* type,
    mg_map* properties);

inline mg_path* mg_path_make(uint32_t node_count, mg_node** nodes,
    uint32_t relationship_count,
    mg_unbound_relationship** relationships,
    uint32_t sequence_length, const int64_t* const sequence);

inline mg_value* mg_value_copy_ca(const mg_value* val, mg_allocator* allocator);

inline mg_string* mg_string_copy_ca(const mg_string* string, mg_allocator* allocator);

inline mg_list* mg_list_copy_ca(const mg_list* list, mg_allocator* allocator);

inline mg_map* mg_map_copy_ca(const mg_map* map, mg_allocator* allocator);

inline mg_node* mg_node_copy_ca(const mg_node* node, mg_allocator* allocator);

inline mg_relationship* mg_relationship_copy_ca(const mg_relationship* rel,
    mg_allocator* allocator);

inline mg_unbound_relationship* mg_unbound_relationship_copy_ca(
    const mg_unbound_relationship* rel, mg_allocator* allocator);

inline mg_path* mg_path_copy_ca(const mg_path* path, mg_allocator* allocator);

inline mg_date* mg_date_copy_ca(const mg_date* date, mg_allocator* allocator);

inline mg_time* mg_time_copy_ca(const mg_time* time, mg_allocator* allocator);

inline mg_local_time* mg_local_time_copy_ca(const mg_local_time* local_time,
    mg_allocator* allocator);

inline mg_date_time* mg_date_time_copy_ca(const mg_date_time* date_time,
    mg_allocator* allocator);

inline mg_date_time_zone_id* mg_date_time_zone_id_copy_ca(
    const mg_date_time_zone_id* date_time_zone_id, mg_allocator* allocator);

inline mg_local_date_time* mg_local_date_time_copy_ca(
    const mg_local_date_time* local_date_time, mg_allocator* allocator);

inline mg_duration* mg_duration_copy_ca(const mg_duration* duration,
    mg_allocator* allocator);

inline mg_point_2d* mg_point_2d_copy_ca(const mg_point_2d* point_2d,
    mg_allocator* allocator);

inline mg_point_3d* mg_point_3d_copy_ca(const mg_point_3d* point_3d,
    mg_allocator* allocator);

inline void mg_path_destroy_ca(mg_path* path, mg_allocator* allocator);

inline void mg_value_destroy_ca(mg_value* val, mg_allocator* allocator);

inline void mg_string_destroy_ca(mg_string* string, mg_allocator* allocator);

inline void mg_list_destroy_ca(mg_list* list, mg_allocator* allocator);

inline void mg_map_destroy_ca(mg_map* map, mg_allocator* allocator);

inline void mg_node_destroy_ca(mg_node* node, mg_allocator* allocator);

inline void mg_relationship_destroy_ca(mg_relationship* rel, mg_allocator* allocator);

inline void mg_unbound_relationship_destroy_ca(mg_unbound_relationship* rel,
    mg_allocator* allocator);

inline void mg_path_destroy_ca(mg_path* path, mg_allocator* allocator);

inline void mg_date_destroy_ca(mg_date* date, mg_allocator* allocator);

inline void mg_time_destroy_ca(mg_time* time, mg_allocator* allocator);

inline void mg_local_time_destroy_ca(mg_local_time* local_time,
    mg_allocator* allocator);

inline void mg_date_time_destroy_ca(mg_date_time* date_time, mg_allocator* allocator);

inline void mg_date_time_zone_id_destroy_ca(mg_date_time_zone_id* date_time_zone_id,
    mg_allocator* allocator);

inline void mg_local_date_time_destroy_ca(mg_local_date_time* local_date_time,
    mg_allocator* allocator);

inline void mg_duration_destroy_ca(mg_duration* duration, mg_allocator* allocator);

inline void mg_point_2d_destroy_ca(mg_point_2d* point_2d, mg_allocator* allocator);

inline void mg_point_3d_destroy_ca(mg_point_3d* point_3d, mg_allocator* allocator);

inline int mg_string_equal(const mg_string* lhs, const mg_string* rhs);

inline int mg_map_equal(const mg_map* lhs, const mg_map* rhs);

inline int mg_node_equal(const mg_node* lhs, const mg_node* rhs);

inline int mg_relationship_equal(const mg_relationship* lhs,
    const mg_relationship* rhs);

inline int mg_unbound_relationship_equal(const mg_unbound_relationship* lhs,
    const mg_unbound_relationship* rhs);

inline int mg_path_equal(const mg_path* lhs, const mg_path* rhs);

inline int mg_value_equal(const mg_value* lhs, const mg_value* rhs);

inline mg_map* mg_default_pull_extra_map;

inline mg_map mg_empty_map = { 0, 0, NULL, NULL };

//mgmessage.h-------------------------------------------------------------------------

// Some of these message types are never sent/received by client, but we still
// have them here for testing.
enum mg_message_type {
    MG_MESSAGE_TYPE_RECORD,
    MG_MESSAGE_TYPE_SUCCESS,
    MG_MESSAGE_TYPE_FAILURE,
    MG_MESSAGE_TYPE_INIT,
    MG_MESSAGE_TYPE_HELLO,
    MG_MESSAGE_TYPE_RUN,
    MG_MESSAGE_TYPE_ACK_FAILURE,
    MG_MESSAGE_TYPE_RESET,
    MG_MESSAGE_TYPE_PULL,
    MG_MESSAGE_TYPE_BEGIN,
    MG_MESSAGE_TYPE_COMMIT,
    MG_MESSAGE_TYPE_ROLLBACK
};

typedef struct mg_message_success {
    mg_map* metadata;
} mg_message_success;

typedef struct mg_message_failure {
    mg_map* metadata;
} mg_message_failure;

typedef struct mg_message_record {
    mg_list* fields;
} mg_message_record;

typedef struct mg_message_init {
    mg_string* client_name;
    mg_map* auth_token;
} mg_message_init;

typedef struct mg_message_hello {
    mg_map* extra;
} mg_message_hello;

typedef struct mg_message_run {
    mg_string* statement;
    mg_map* parameters;
    mg_map* extra;
} mg_message_run;

typedef struct mg_message_begin {
    mg_map* extra;
} mg_message_begin;

typedef struct mg_message_pull {
    mg_map* extra;
} mg_message_pull;

typedef struct mg_message {
    enum mg_message_type type;
    union {
        mg_message_success* success_v;
        mg_message_failure* failure_v;
        mg_message_record* record_v;
        mg_message_init* init_v;
        mg_message_hello* hello_v;
        mg_message_run* run_v;
        mg_message_begin* begin_v;
        mg_message_pull* pull_v;
    };
} mg_message;

inline void mg_message_destroy_ca(mg_message* message, mg_allocator* allocator);

//mgcommon.h-------------------------------------------------------------------------

#define htobe16(x) _byteswap_ushort(x)
#define htole16(x) (x)
#define be16toh(x) _byteswap_ushort(x)
#define le16toh(x) (x)

#define htobe32(x) _byteswap_ulong(x)
#define htole32(x) (x)
#define be32toh(x) _byteswap_ulong(x)
#define le32toh(x) (x)

#define htobe64(x) _byteswap_uint64(x)
#define htole64(x) (x)
#define be64toh(x) _byteswap_uint64(x)
#define le64toh(x) (x)

#define MG_RETURN_IF_FAILED(expression) \
  do {                                  \
    int status = (expression);          \
    if (status != 0) {                  \
      return status;                    \
    }                                   \
  } while (0)

#ifdef NDEBUG
#define DB_ACTIVE 0
#else
#define DB_ACTIVE 1
#endif  // NDEBUG
#define DB_LOG(x)                      \
  do {                                 \
    if (DB_ACTIVE) fprintf(stderr, x); \
  } while (0)

#define MG_ATTRIBUTE_WEAK

//mgtransport.h-------------------------------------------------------------------------

typedef struct mg_transport {
    int (*send)(struct mg_transport*, const char* buf, size_t len);
    int (*recv)(struct mg_transport*, char* buf, size_t len);
    void (*destroy)(struct mg_transport*);
    void (*suspend_until_ready_to_read)(struct mg_transport*);
    void (*suspend_until_ready_to_write)(struct mg_transport*);
} mg_transport;

typedef struct mg_raw_transport {
    int (*send)(struct mg_transport*, const char* buf, size_t len);
    int (*recv)(struct mg_transport*, char* buf, size_t len);
    void (*destroy)(struct mg_transport*);
    void (*suspend_until_ready_to_read)(struct mg_transport*);
    void (*suspend_until_ready_to_write)(struct mg_transport*);
    int sockfd;
    mg_allocator* allocator;
} mg_raw_transport;

#ifdef MGCLIENT_ENABLE_SSL
typedef struct mg_secure_transport {
    int (*send)(struct mg_transport*, const char* buf, size_t len);
    int (*recv)(struct mg_transport*, char* buf, size_t len);
    void (*destroy)(struct mg_transport*);
    void (*suspend_until_ready_to_read)(struct mg_transport*);
    void (*suspend_until_ready_to_write)(struct mg_transport*);
    SSL* ssl;
    BIO* bio;
    const char* peer_pubkey_type;
    char* peer_pubkey_fp;
    mg_allocator* allocator;
} mg_secure_transport;
#endif

inline int mg_transport_send(mg_transport* transport, const char* buf, size_t len);

inline int mg_transport_recv(mg_transport* transport, char* buf, size_t len);

inline void mg_transport_destroy(mg_transport* transport);

inline void mg_transport_suspend_until_ready_to_read(struct mg_transport*);

inline void mg_transport_suspend_until_ready_to_write(struct mg_transport*);

inline int mg_raw_transport_init(int sockfd, mg_raw_transport** transport,
    mg_allocator* allocator);

inline int mg_raw_transport_send(struct mg_transport*, const char* buf, size_t len);

inline int mg_raw_transport_recv(struct mg_transport*, char* buf, size_t len);

inline void mg_raw_transport_destroy(struct mg_transport*);

inline void mg_raw_transport_suspend_until_ready_to_read(struct mg_transport*);

inline void mg_raw_transport_suspend_until_ready_to_write(struct mg_transport*);

#ifdef MGCLIENT_ENABLE_SSL
// This function is mocked in tests during linking by using --wrap. ON_APPLE
// there is no --wrap. An alternative is to use -alias but if a symbol is
// strong linking fails.
MG_ATTRIBUTE_WEAK inline int mg_secure_transport_init(int sockfd,
    const char* cert_file,
    const char* key_file,
    mg_secure_transport** transport,
    mg_allocator* allocator);

inline int mg_secure_transport_send(mg_transport*, const char* buf, size_t len);

inline int mg_secure_transport_recv(mg_transport*, char* buf, size_t len);

inline void mg_secure_transport_destroy(mg_transport*);
#endif

//mgsession.h-------------------------------------------------------------------------

#define MG_MAX_ERROR_SIZE 1024

typedef struct mg_result {
    int status;
    mg_session* session;
    mg_message* message;
    mg_list* columns;
} mg_result;

typedef struct mg_session {
    int status;

    int explicit_transaction;
    int query_number;

    mg_transport* transport;

    int version;

    char* out_buffer;
    size_t out_begin;
    size_t out_end;
    size_t out_capacity;

    char* in_buffer;
    size_t in_end;
    size_t in_capacity;
    size_t in_cursor;

    mg_result result;

    char error_buffer[MG_MAX_ERROR_SIZE];

    mg_allocator* allocator;
    mg_allocator* decoder_allocator;
} mg_session;

inline mg_session* mg_session_init(mg_allocator* allocator);

inline void mg_session_invalidate(mg_session* session);

inline void mg_session_set_error(mg_session* session, const char* fmt, ...);

inline void mg_session_destroy(mg_session* session);

inline int mg_session_write_raw(mg_session* session, const char* data, size_t len);

inline int mg_session_flush_message(mg_session* session);

inline int mg_session_write_uint8(mg_session* session, uint8_t val);

inline int mg_session_write_uint16(mg_session* session, uint16_t val);

inline int mg_session_write_uint32(mg_session* session, uint32_t val);

inline int mg_session_write_uint64(mg_session* session, uint64_t val);

inline int mg_session_write_null(mg_session* session);

inline int mg_session_write_bool(mg_session* session, int value);

inline int mg_session_write_integer(mg_session* session, int64_t value);

inline int mg_session_write_float(mg_session* session, double value);

inline int mg_session_write_string(mg_session* session, const char* str);

inline int mg_session_write_string2(mg_session* session, uint32_t len,
    const char* data);

inline int mg_session_write_list(mg_session* session, const mg_list* list);

inline int mg_session_write_map(mg_session* session, const mg_map* map);

inline int mg_session_write_value(mg_session* session, const mg_value* value);

inline int mg_session_receive_message(mg_session* session);

inline void* mg_session_allocate(mg_session* session, size_t size);

inline int mg_session_read_integer(mg_session* session, int64_t* val);

inline int mg_session_read_bool(mg_session* session, int* val);

inline int mg_session_read_float(mg_session* session, double* value);

inline int mg_session_read_string(mg_session* session, mg_string** str);

inline int mg_session_read_list(mg_session* session, mg_list** list);

inline int mg_session_read_map(mg_session* session, mg_map** map);

inline int mg_session_read_node(mg_session* session, mg_node** node);

inline int mg_session_read_relationship(mg_session* session, mg_relationship** rel);

inline int mg_session_read_unbound_relationship(mg_session* session,
    mg_unbound_relationship** rel);

inline int mg_session_read_path(mg_session* session, mg_path** path);

inline int mg_session_read_date(mg_session* session, mg_date** date);

inline int mg_session_read_time(mg_session* session, mg_time** time);

inline int mg_session_read_local_time(mg_session* session, mg_local_time** local_time);

inline int mg_session_read_date_time(mg_session* session, mg_date_time** date_time);

inline int mg_session_read_date_time_zone_id(mg_session* session,
    mg_date_time_zone_id** date_time_zone_id);

inline int mg_session_read_local_date_time(mg_session* session,
    mg_local_date_time** local_date_time);

inline int mg_session_read_duration(mg_session* session, mg_duration** duration);

inline int mg_session_read_point_2d(mg_session* session, mg_point_2d** point_2d);

inline int mg_session_read_point_3d(mg_session* session, mg_point_3d** point_3d);

inline int mg_session_read_value(mg_session* session, mg_value** value);

inline int mg_session_read_bolt_message(mg_session* session, mg_message** message);

// Some of these message types are never sent by client, but send functions are
// still here for testing.
inline int mg_session_send_init_message(mg_session* session, const char* client_name,
    const mg_map* auth_token);

inline int mg_session_send_hello_message(mg_session* session, const mg_map* extra);

inline int mg_session_send_run_message(mg_session* session, const char* statement,
    const mg_map* parameters, const mg_map* extra);

inline int mg_session_send_pull_message(mg_session* session, const mg_map* extra);

inline int mg_session_send_reset_message(mg_session* session);

inline int mg_session_send_ack_failure_message(mg_session* session);

inline int mg_session_send_failure_message(mg_session* session,
    const mg_map* metadata);

inline int mg_session_send_success_message(mg_session* session,
    const mg_map* metadata);

inline int mg_session_send_record_message(mg_session* session, const mg_list* fields);

inline int mg_session_send_begin_message(mg_session* session, const mg_map* extra);

inline int mg_session_send_commit_messsage(mg_session* session);

inline int mg_session_send_rollback_messsage(mg_session* session);

//mgsocket.h-------------------------------------------------------------------------

#include <Ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>

#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef long ssize_t;
#endif

/// Initializes underlying resources. Has to be called at the beginning of a
/// process using socket resources.
inline int mg_socket_init();

/// Returns a descriptor referencing the new socket or MG_ERROR_SOCKET in the
/// case of any failure.
inline int mg_socket_create(int af, int type, int protocol);

/// Checks for errors after \ref mg_socket_create call.
///
/// \param[in]  sock    Return value out of \ref mg_socket_create call.
/// \param[in]  session A pointer to the session object to set the error
///                     message if required.
///
/// \return \ref MG_ERROR in case of an error or \ref MG_SUCCESS if there is no
/// error. In the error case, session will have the underlying error message
/// set.
inline int mg_socket_create_handle_error(int sock, mg_session* session);

/// Connects the socket referred to by the sock descriptor to the address
/// specified by addr. The addrlen argument specifies the size of addr.
///
/// \return \ref MG_ERROR in case of an error or \ref MG_SUCCESS if there is no
/// error.
inline int mg_socket_connect(int sock, const struct sockaddr* addr, socklen_t addrlen);

/// Checks for errors after \ref mg_socket_connect call.
///
/// \param[out] sock    Return value out of \ref mg_socket_create call.
/// \param[in]  status  Return value out of \ref mg_socket_connect call.
/// \param[in]  session A pointer to the session object to set the error
///                     message if required.
///
/// \return \ref MG_ERROR in case of an error or \ref MG_SUCCESS if there is no
/// error. In the error case, session will have the underlying error message
/// set + value referenced by the sock will be set to MG_ERROR_SOCKET.
inline int mg_socket_connect_handle_error(int* sock, int status, mg_session* session);

/// Sets options for a socket referenced with the given sock descriptor.
inline int mg_socket_options(int sock, mg_session* session);

/// Sends len bytes from buf to the socket referenced by the sock descriptor.
inline ssize_t mg_socket_send(int sock, const void* buf, int len);

/// Reads len bytes to buf from the socket referenced by the sock descriptor.
inline ssize_t mg_socket_receive(int sock, void* buf, int len);

/// Waits for one of a set of file descriptors to become ready to perform I/O.
inline int mg_socket_poll(struct pollfd* fds, unsigned int nfds, int timeout);

/// Creates a socket pair.
inline int mg_socket_pair(int d, int type, int protocol, int* sv);

/// Closes the socket referenced by the sock descriptor.
inline int mg_socket_close(int sock);

/// Used to get a native error message after some socket call fails.
/// Has to be called immediately after the failed socket function.
inline char* mg_socket_error();

/// Should be called at the end of any process which previously called the
/// \ref mg_socket_init function.
inline void mg_socket_finalize();

//mgallocator.c-------------------------------------------------------------------------

inline void* mg_system_realloc(struct mg_allocator* self, void* buf, size_t size) {
    (void)self;
    return realloc(buf, size);
}

inline void* mg_system_malloc(struct mg_allocator* self, size_t size) {
    (void)self;
    return malloc(size);
}

inline void mg_system_free(struct mg_allocator* self, void* buf) {
    (void)self;
    free(buf);
}

inline void* mg_allocator_malloc(struct mg_allocator* allocator, size_t size) {
    return allocator->malloc(allocator, size);
}

inline void* mg_allocator_realloc(struct mg_allocator* allocator, void* buf,
    size_t size) {
    return allocator->realloc(allocator, buf, size);
}

inline void mg_allocator_free(struct mg_allocator* allocator, void* buf) {
    allocator->free(allocator, buf);
}

inline struct mg_allocator mg_system_allocator = { mg_system_malloc, mg_system_realloc,
                                           mg_system_free };

typedef struct mg_memory_block {
    char* buffer;
    struct mg_memory_block* next;
} mg_memory_block;

inline mg_memory_block* mg_memory_block_alloc(mg_allocator* allocator, size_t size) {
    /// Ensure that the memory is properly aligned.
    static_assert(
        sizeof(mg_memory_block) % alignof(max_align_t) == 0,
        "Size of mg_memory_block doesn't satisfy alignment requirements");
    mg_memory_block* block =
        (mg_memory_block*)mg_allocator_malloc(allocator, sizeof(mg_memory_block) + size);
    if (!block) {
        return NULL;
    }
    block->next = NULL;
    block->buffer = (char*)block + sizeof(mg_memory_block);
    return block;
}

typedef struct mg_linear_allocator {
    void* (*malloc)(struct mg_allocator* self, size_t size);
    void* (*realloc)(struct mg_allocator* self, void* buf, size_t size);
    void (*free)(struct mg_allocator* self, void* buf);

    mg_memory_block* current_block;
    size_t current_offset;

    const size_t block_size;
    const size_t sep_alloc_threshold;

    mg_allocator* underlying_allocator;
} mg_linear_allocator;

inline void* mg_linear_allocator_malloc(struct mg_allocator* allocator, size_t size) {
    mg_linear_allocator* self = (mg_linear_allocator*)allocator;

    if (size >= self->sep_alloc_threshold) {
        // Make a new block, but put it below the first block so we don't waste
        // bytes in it.
        mg_memory_block* new_block =
            mg_memory_block_alloc(self->underlying_allocator, size);
        new_block->next = self->current_block->next;
        self->current_block->next = new_block;
        return new_block->buffer;
    }

    if (self->current_offset + size > self->block_size) {
        // Create a new block and put it at the beginning of the list.
        mg_memory_block* new_block =
            mg_memory_block_alloc(self->underlying_allocator, self->block_size);
        new_block->next = self->current_block;
        self->current_block = new_block;
        self->current_offset = 0;
    }

    assert(self->current_offset + size <= self->block_size);
    assert(self->current_offset % alignof(max_align_t) == 0);

    void* ret = self->current_block->buffer + self->current_offset;
    self->current_offset += size;
    if (self->current_offset % alignof(max_align_t) != 0) {
        self->current_offset +=
            alignof(max_align_t) - (self->current_offset % alignof(max_align_t));
    }
    return ret;
}

inline void* mg_linear_allocator_realloc(struct mg_allocator* allocator, void* buf,
    size_t size) {
    (void)allocator;
    (void)buf;
    (void)size;
    fprintf(stderr, "mg_linear_allocator doesn't support realloc\n");
    return NULL;
}

inline void mg_linear_allocator_free(struct mg_allocator* allocator, void* buf) {
    (void)allocator;
    (void)buf;
    return;
}

inline mg_linear_allocator* mg_linear_allocator_init(mg_allocator* allocator,
    size_t block_size,
    size_t sep_alloc_threshold) {
    mg_memory_block* first_block = mg_memory_block_alloc(allocator, block_size);
    if (!first_block) {
        return NULL;
    }

    mg_linear_allocator tmp_alloc = { mg_linear_allocator_malloc,
                                     mg_linear_allocator_realloc,
                                     mg_linear_allocator_free,
                                     first_block,
                                     0,
                                     block_size,
                                     sep_alloc_threshold,
                                     allocator };
    mg_linear_allocator* alloc =
        (mg_linear_allocator*)mg_allocator_malloc(allocator, sizeof(mg_linear_allocator));
    if (!alloc) {
        mg_allocator_free(allocator, first_block);
        return NULL;
    }

    memcpy(alloc, &tmp_alloc, sizeof(mg_linear_allocator));
    return alloc;
}

inline void mg_linear_allocator_destroy(mg_linear_allocator* allocator) {
    if (allocator == NULL) {
        return;
    }
    while (allocator->current_block) {
        mg_memory_block* next_block = allocator->current_block->next;
        mg_allocator_free(allocator->underlying_allocator,
            allocator->current_block);
        allocator->current_block = next_block;
    }
    mg_allocator_free(allocator->underlying_allocator, allocator);
}

inline void mg_linear_allocator_reset(mg_linear_allocator* allocator) {
    // The first block is always of size allocator->block_size. We will keep that
    // one and free the others.
    mg_memory_block* first_block = allocator->current_block;
    while (first_block->next) {
        mg_memory_block* new_next = first_block->next->next;
        mg_allocator_free(allocator->underlying_allocator, first_block->next);
        first_block->next = new_next;
    }
    allocator->current_offset = 0;
}

//mgvalue.c-------------------------------------------------------------------------

inline mg_string* mg_string_alloc(uint32_t size, mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_string) + size);
    if (!block) {
        return NULL;
    }
    mg_string* str = (mg_string*)block;
    str->data = block + sizeof(mg_string);
    return str;
}

inline mg_list* mg_list_alloc(uint32_t size, mg_allocator* allocator) {
    size_t elements_size = size * sizeof(mg_value*);
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_list) + elements_size);
    if (!block) {
        return NULL;
    }
    mg_list* list = (mg_list*)block;
    list->elements = (mg_value**)(block + sizeof(mg_list));
    return list;
}

inline mg_map* mg_map_alloc(uint32_t size, mg_allocator* allocator) {
    size_t keys_size = size * sizeof(mg_string*);
    size_t values_size = size * sizeof(mg_value*);
    char* block =
        (char*)mg_allocator_malloc(allocator, sizeof(mg_map) + keys_size + values_size);
    if (!block) {
        return NULL;
    }
    mg_map* map = (mg_map*)block;
    map->keys = (mg_string**)(block + sizeof(mg_map));
    map->values = (mg_value**)(block + sizeof(mg_map) + keys_size);
    return map;
}

inline mg_node* mg_node_alloc(uint32_t label_count, mg_allocator* allocator) {
    size_t labels_size = label_count * sizeof(mg_string*);
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_node) + labels_size);
    if (!block) {
        return NULL;
    }
    mg_node* node = (mg_node*)block;
    node->labels = (mg_string**)(block + sizeof(mg_node));
    return node;
}

inline mg_path* mg_path_alloc(uint32_t node_count, uint32_t relationship_count,
    uint32_t sequence_length, mg_allocator* allocator) {
    size_t nodes_size = node_count * sizeof(mg_node*);
    size_t relationships_size =
        relationship_count * sizeof(mg_unbound_relationship*);
    size_t sequence_size = sequence_length * sizeof(int64_t);
    char* block =
        (char*)mg_allocator_malloc(allocator, sizeof(mg_path) + nodes_size +
            relationships_size + sequence_size);
    if (!block) {
        return NULL;
    }
    mg_path* path = (mg_path*)block;
    path->nodes = (mg_node**)(block + sizeof(mg_path));
    path->relationships =
        (mg_unbound_relationship**)(block + sizeof(mg_path) + nodes_size);
    path->sequence =
        (int64_t*)(block + sizeof(mg_path) + nodes_size + relationships_size);
    return path;
}

inline mg_date* mg_date_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_date));

    if (!block) {
        return NULL;
    }
    mg_date* date = (mg_date*)block;
    return date;
}

inline mg_time* mg_time_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_time));

    if (!block) {
        return NULL;
    }
    mg_time* time = (mg_time*)block;
    return time;
}

inline mg_local_time* mg_local_time_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_local_time));

    if (!block) {
        return NULL;
    }
    mg_local_time* local_time = (mg_local_time*)block;
    return local_time;
}

inline mg_date_time* mg_date_time_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_date_time));

    if (!block) {
        return NULL;
    }
    mg_date_time* date_time = (mg_date_time*)block;
    return date_time;
}

inline mg_date_time_zone_id* mg_date_time_zone_id_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_date_time_zone_id));

    if (!block) {
        return NULL;
    }
    mg_date_time_zone_id* date_time_zone_id = (mg_date_time_zone_id*)block;
    return date_time_zone_id;
}

inline mg_local_date_time* mg_local_date_time_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_local_date_time));

    if (!block) {
        return NULL;
    }
    mg_local_date_time* local_date_time = (mg_local_date_time*)block;
    return local_date_time;
}

inline mg_duration* mg_duration_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_duration));

    if (!block) {
        return NULL;
    }
    mg_duration* duration = (mg_duration*)block;
    return duration;
}

inline mg_point_2d* mg_point_2d_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_point_2d));

    if (!block) {
        return NULL;
    }
    mg_point_2d* point_2d = (mg_point_2d*)block;
    return point_2d;
}

inline mg_point_3d* mg_point_3d_alloc(mg_allocator* allocator) {
    char* block = (char*)mg_allocator_malloc(allocator, sizeof(mg_point_3d));

    if (!block) {
        return NULL;
    }
    mg_point_3d* point_3d = (mg_point_3d*)block;
    return point_3d;
}

inline mg_value* mg_value_make_null() {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_NULL;
    return value;
}

inline mg_value* mg_value_make_bool(int val) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_BOOL;
    value->bool_v = val;
    return value;
}

inline mg_value* mg_value_make_integer(int64_t val) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_INTEGER;
    value->integer_v = val;
    return value;
}

inline mg_value* mg_value_make_float(double val) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_FLOAT;
    value->float_v = val;
    return value;
}

inline mg_value* mg_value_make_string(const char* str) {
    mg_string* tstr = mg_string_make(str);
    if (!tstr) {
        return NULL;
    }
    return mg_value_make_string2(tstr);
}

inline mg_value* mg_value_make_string2(mg_string* str) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_STRING;
    value->string_v = str;
    return value;
}

inline mg_value* mg_value_make_list(mg_list* list) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_LIST;
    value->list_v = list;
    return value;
}

inline mg_value* mg_value_make_map(mg_map* map) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_MAP;
    value->map_v = map;
    return value;
}

inline mg_value* mg_value_make_node(mg_node* node) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_NODE;
    value->node_v = node;
    return value;
}

inline mg_value* mg_value_make_relationship(mg_relationship* rel) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_RELATIONSHIP;
    value->relationship_v = rel;
    return value;
}

inline mg_value* mg_value_make_unbound_relationship(mg_unbound_relationship* rel) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_UNBOUND_RELATIONSHIP;
    value->unbound_relationship_v = rel;
    return value;
}

inline mg_value* mg_value_make_path(mg_path* path) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_PATH;
    value->path_v = path;
    return value;
}

inline mg_value* mg_value_make_date(mg_date* date) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_DATE;
    value->date_v = date;
    return value;
}

inline mg_value* mg_value_make_time(mg_time* time) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_TIME;
    value->time_v = time;
    return value;
}

inline mg_value* mg_value_make_local_time(mg_local_time* local_time) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_LOCAL_TIME;
    value->local_time_v = local_time;
    return value;
}

inline mg_value* mg_value_make_date_time(mg_date_time* date_time) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_DATE_TIME;
    value->date_time_v = date_time;
    return value;
}

inline mg_value* mg_value_make_date_time_zone_id(
    mg_date_time_zone_id* date_time_zone_id) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_DATE_TIME_ZONE_ID;
    value->date_time_zone_id_v = date_time_zone_id;
    return value;
}

inline mg_value* mg_value_make_local_date_time(mg_local_date_time* local_date_time) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_LOCAL_DATE_TIME;
    value->local_date_time_v = local_date_time;
    return value;
}

inline mg_value* mg_value_make_duration(mg_duration* duration) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_DURATION;
    value->duration_v = duration;
    return value;
}

inline mg_value* mg_value_make_point_2d(mg_point_2d* point_2d) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_POINT_2D;
    value->point_2d_v = point_2d;
    return value;
}

inline mg_value* mg_value_make_point_3d(mg_point_3d* point_3d) {
    mg_value* value = (mg_value*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_value));
    if (!value) {
        return NULL;
    }
    value->type = MG_VALUE_TYPE_POINT_3D;
    value->point_3d_v = point_3d;
    return value;
}

inline enum mg_value_type mg_value_get_type(const mg_value* val) { return val->type; }

inline int mg_value_bool(const mg_value* val) { return val->bool_v; }

inline int64_t mg_value_integer(const mg_value* val) { return val->integer_v; }

inline double mg_value_float(const mg_value* val) { return val->float_v; }

inline const mg_string* mg_value_string(const mg_value* val) { return val->string_v; }

inline const mg_list* mg_value_list(const mg_value* val) { return val->list_v; }

inline const mg_map* mg_value_map(const mg_value* val) { return val->map_v; }

inline const mg_node* mg_value_node(const mg_value* val) { return val->node_v; }

inline const mg_relationship* mg_value_relationship(const mg_value* val) {
    return val->relationship_v;
}

inline const mg_unbound_relationship* mg_value_unbound_relationship(
    const mg_value* val) {
    return val->unbound_relationship_v;
}

inline const mg_path* mg_value_path(const mg_value* val) { return val->path_v; }

inline const mg_date* mg_value_date(const mg_value* val) { return val->date_v; }

inline const mg_time* mg_value_time(const mg_value* val) { return val->time_v; }

inline const mg_local_time* mg_value_local_time(const mg_value* val) {
    return val->local_time_v;
}

inline const mg_date_time* mg_value_date_time(const mg_value* val) {
    return val->date_time_v;
}

inline const mg_date_time_zone_id* mg_value_date_time_zone_id(const mg_value* val) {
    return val->date_time_zone_id_v;
}

inline const mg_local_date_time* mg_value_local_date_time(const mg_value* val) {
    return val->local_date_time_v;
}

inline const mg_duration* mg_value_duration(const mg_value* val) {
    return val->duration_v;
}

inline const mg_point_2d* mg_value_point_2d(const mg_value* val) {
    return val->point_2d_v;
}

inline const mg_point_3d* mg_value_point_3d(const mg_value* val) {
    return val->point_3d_v;
}

inline mg_value* mg_value_copy_ca(const mg_value* val, mg_allocator* allocator) {
    if (!val) {
        return NULL;
    }
    mg_value* new_val = (mg_value*)mg_allocator_malloc(allocator, sizeof(mg_value));
    if (!new_val) {
        return NULL;
    }
    new_val->type = val->type;
    switch (val->type) {
        case MG_VALUE_TYPE_NULL:
            break;
        case MG_VALUE_TYPE_BOOL:
            new_val->bool_v = val->bool_v;
            break;
        case MG_VALUE_TYPE_INTEGER:
            new_val->integer_v = val->integer_v;
            break;
        case MG_VALUE_TYPE_FLOAT:
            new_val->float_v = val->float_v;
            break;
        case MG_VALUE_TYPE_STRING:
            new_val->string_v = mg_string_copy_ca(val->string_v, allocator);
            if (!new_val->string_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_LIST:
            new_val->list_v = mg_list_copy_ca(val->list_v, allocator);
            if (!new_val->list_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_MAP:
            new_val->map_v = mg_map_copy_ca(val->map_v, allocator);
            if (!new_val->map_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_NODE:
            new_val->node_v = mg_node_copy_ca(val->node_v, allocator);
            if (!new_val->node_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_RELATIONSHIP:
            new_val->relationship_v =
                mg_relationship_copy_ca(val->relationship_v, allocator);
            if (!new_val->relationship_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_UNBOUND_RELATIONSHIP:
            new_val->unbound_relationship_v = mg_unbound_relationship_copy_ca(
                val->unbound_relationship_v, allocator);
            if (!new_val->unbound_relationship_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_PATH:
            new_val->path_v = mg_path_copy_ca(val->path_v, allocator);
            if (!new_val->path_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_DATE:
            new_val->date_v = mg_date_copy_ca(val->date_v, allocator);
            if (!new_val->date_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_TIME:
            new_val->time_v = mg_time_copy_ca(val->time_v, allocator);
            if (!new_val->time_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_LOCAL_TIME:
            new_val->local_time_v =
                mg_local_time_copy_ca(val->local_time_v, allocator);
            if (!new_val->local_time_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_DATE_TIME:
            new_val->date_time_v = mg_date_time_copy_ca(val->date_time_v, allocator);
            if (!new_val->date_time_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_DATE_TIME_ZONE_ID:
            new_val->date_time_zone_id_v =
                mg_date_time_zone_id_copy_ca(val->date_time_zone_id_v, allocator);
            if (!new_val->date_time_zone_id_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_LOCAL_DATE_TIME:
            new_val->local_date_time_v =
                mg_local_date_time_copy_ca(val->local_date_time_v, allocator);
            if (!new_val->local_date_time_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_DURATION:
            new_val->duration_v = mg_duration_copy_ca(val->duration_v, allocator);
            if (!new_val->duration_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_POINT_2D:
            new_val->point_2d_v = mg_point_2d_copy_ca(val->point_2d_v, allocator);
            if (!new_val->point_2d_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_POINT_3D:
            new_val->point_3d_v = mg_point_3d_copy_ca(val->point_3d_v, allocator);
            if (!new_val->point_3d_v) {
                goto cleanup;
            }
            break;
        case MG_VALUE_TYPE_UNKNOWN:
            break;
    }
    return new_val;

cleanup:
    mg_allocator_free(&mg_system_allocator, new_val);
    return NULL;
}

inline mg_value* mg_value_copy(const mg_value* val) {
    return mg_value_copy_ca(val, &mg_system_allocator);
}

inline void mg_value_destroy_ca(mg_value* val, mg_allocator* allocator) {
    if (!val) {
        return;
    }
    switch (val->type) {
        case MG_VALUE_TYPE_NULL:
        case MG_VALUE_TYPE_BOOL:
        case MG_VALUE_TYPE_INTEGER:
        case MG_VALUE_TYPE_FLOAT:
            break;
        case MG_VALUE_TYPE_DATE:
            mg_date_destroy_ca(val->date_v, allocator);
            break;
        case MG_VALUE_TYPE_TIME:
            mg_time_destroy_ca(val->time_v, allocator);
            break;
        case MG_VALUE_TYPE_LOCAL_TIME:
            mg_local_time_destroy_ca(val->local_time_v, allocator);
            break;
        case MG_VALUE_TYPE_DATE_TIME:
            mg_date_time_destroy_ca(val->date_time_v, allocator);
            break;
        case MG_VALUE_TYPE_DATE_TIME_ZONE_ID:
            mg_date_time_zone_id_destroy_ca(val->date_time_zone_id_v, allocator);
            break;
        case MG_VALUE_TYPE_LOCAL_DATE_TIME:
            mg_local_date_time_destroy_ca(val->local_date_time_v, allocator);
            break;
        case MG_VALUE_TYPE_DURATION:
            mg_duration_destroy_ca(val->duration_v, allocator);
            break;
        case MG_VALUE_TYPE_POINT_2D:
            mg_point_2d_destroy_ca(val->point_2d_v, allocator);
            break;
        case MG_VALUE_TYPE_POINT_3D:
            mg_point_3d_destroy_ca(val->point_3d_v, allocator);
            break;
        case MG_VALUE_TYPE_STRING:
            mg_string_destroy_ca(val->string_v, allocator);
            break;
        case MG_VALUE_TYPE_LIST:
            mg_list_destroy_ca(val->list_v, allocator);
            break;
        case MG_VALUE_TYPE_MAP:
            mg_map_destroy_ca(val->map_v, allocator);
            break;
        case MG_VALUE_TYPE_NODE:
            mg_node_destroy_ca(val->node_v, allocator);
            break;
        case MG_VALUE_TYPE_RELATIONSHIP:
            mg_relationship_destroy_ca(val->relationship_v, allocator);
            break;
        case MG_VALUE_TYPE_UNBOUND_RELATIONSHIP:
            mg_unbound_relationship_destroy_ca(val->unbound_relationship_v,
                allocator);
            break;
        case MG_VALUE_TYPE_PATH:
            mg_path_destroy_ca(val->path_v, allocator);
            break;
        case MG_VALUE_TYPE_UNKNOWN:
            break;
    }
    mg_allocator_free(allocator, val);
}

inline void mg_value_destroy(mg_value* val) {
    if (!val) {
        return;
    }
    mg_value_destroy_ca(val, &mg_system_allocator);
}

inline mg_string* mg_string_make(const char* str) {
    size_t size = strlen(str);
    if (size >= UINT32_MAX) return NULL;
    return mg_string_make2((uint32_t)size, str);
}

inline mg_string* mg_string_make2(uint32_t len, const char* data) {
    mg_string* str = mg_string_alloc(len, &mg_system_allocator);
    if (!str) {
        return NULL;
    }
    str->size = len;
    memcpy(str->data, data, len);
    return str;
}

inline const char* mg_string_data(const mg_string* str) { return str->data; }

inline uint32_t mg_string_size(const mg_string* str) { return str->size; }

inline mg_string* mg_string_copy_ca(const mg_string* src, mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_string* str = mg_string_alloc(src->size, allocator);
    if (!str) {
        return NULL;
    }
    str->size = src->size;
    memcpy(str->data, src->data, src->size);
    return str;
}

inline mg_string* mg_string_copy(const mg_string* str) {
    return mg_string_copy_ca(str, &mg_system_allocator);
}

inline void mg_string_destroy_ca(mg_string* str, mg_allocator* allocator) {
    if (!str) {
        return;
    }
    mg_allocator_free(allocator, str);
}

inline void mg_string_destroy(mg_string* str) {
    mg_string_destroy_ca(str, &mg_system_allocator);
}

inline static int mg_string_eq(uint32_t size1, const char* str1, uint32_t size2,
    const char* str2) {
    if (size1 != size2) return 0;
    return memcmp(str1, str2, size1) == 0;
}

inline mg_list* mg_list_make_empty(uint32_t capacity) {
    mg_list* list = mg_list_alloc(capacity, &mg_system_allocator);
    if (!list) {
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;
}

inline int mg_list_append(mg_list* list, mg_value* value) {
    if (list->size >= list->capacity) {
        return MG_ERROR_CONTAINER_FULL;
    }
    list->elements[list->size] = value;
    list->size++;
    return 0;
}

inline uint32_t mg_list_size(const mg_list* list) { return list->size; }

inline const mg_value* mg_list_at(const mg_list* list, uint32_t pos) {
    if (pos < list->size) {
        return list->elements[pos];
    } else {
        return NULL;
    }
}

inline mg_list* mg_list_copy_ca(const mg_list* list, mg_allocator* allocator) {
    if (!list) {
        return NULL;
    }
    mg_list* nlist = mg_list_alloc(list->size, allocator);
    if (!nlist) {
        return NULL;
    }
    nlist->capacity = list->size;
    nlist->size = 0;
    for (uint32_t i = 0; i < list->size; ++i) {
        nlist->elements[i] = mg_value_copy_ca(list->elements[i], allocator);
        if (!nlist->elements[i]) {
            goto cleanup;
        }
        nlist->size++;
    }
    return nlist;

cleanup:
    for (uint32_t i = 0; i < nlist->size; ++i) {
        mg_value_destroy(nlist->elements[i]);
    }
    mg_allocator_free(allocator, nlist);
    return NULL;
}

inline mg_list* mg_list_copy(const mg_list* list) {
    return mg_list_copy_ca(list, &mg_system_allocator);
}

inline void mg_list_destroy_ca(mg_list* list, mg_allocator* allocator) {
    if (!list) {
        return;
    }
    for (uint32_t i = 0; i < list->size; ++i) {
        mg_value_destroy_ca(list->elements[i], allocator);
    }
    mg_allocator_free(allocator, list);
}

inline void mg_list_destroy(mg_list* list) {
    mg_list_destroy_ca(list, &mg_system_allocator);
}

inline mg_map* mg_map_make_empty(uint32_t capacity) {
    mg_map* map = mg_map_alloc(capacity, &mg_system_allocator);
    if (!map) {
        return NULL;
    }
    map->size = 0;
    map->capacity = capacity;
    return map;
}

inline static uint32_t mg_map_find_key(const mg_map* map, uint32_t key_size,
    const char* key_data) {
    for (uint32_t i = 0; i < map->size; ++i) {
        if (mg_string_eq(map->keys[i]->size, map->keys[i]->data, key_size,
            key_data)) {
            return i;
        }
    }
    return map->size;
}

inline static void mg_map_append(mg_map* map, mg_string* key, mg_value* value) {
    map->keys[map->size] = key;
    map->values[map->size] = value;
    map->size++;
}

inline int mg_map_insert(mg_map* map, const char* key_str, mg_value* value) {
    size_t key_size = strlen(key_str);
    if (key_size >= UINT32_MAX) {
        return MG_ERROR_SIZE_EXCEEDED;
    }
    if (map->size >= map->capacity) {
        return MG_ERROR_CONTAINER_FULL;
    }
    if (mg_map_find_key(map, (uint32_t)key_size, key_str) != map->size) {
        return MG_ERROR_DUPLICATE_KEY;
    }
    mg_string* key = mg_string_make2((uint32_t)key_size, key_str);
    if (!key) {
        return MG_ERROR_OOM;
    }
    mg_map_append(map, key, value);
    return 0;
}

inline int mg_map_insert2(mg_map* map, mg_string* key, mg_value* value) {
    if (map->size >= map->capacity) {
        return MG_ERROR_CONTAINER_FULL;
    }
    if (mg_map_find_key(map, key->size, key->data) != map->size) {
        return MG_ERROR_DUPLICATE_KEY;
    }
    mg_map_append(map, key, value);
    return 0;
}

inline int mg_map_insert_unsafe(mg_map* map, const char* key_str, mg_value* value) {
    if (map->size >= map->capacity) {
        return MG_ERROR_CONTAINER_FULL;
    }
    size_t key_len = strlen(key_str);
    if (key_len >= UINT32_MAX) {
        return MG_ERROR_SIZE_EXCEEDED;
    }
    mg_string* key = mg_string_make2((uint32_t)key_len, key_str);
    if (key == NULL) {
        return MG_ERROR_OOM;
    }
    mg_map_append(map, key, value);
    return 0;
}

inline int mg_map_insert_unsafe2(mg_map* map, mg_string* key, mg_value* value) {
    if (map->size >= map->capacity) {
        return MG_ERROR_CONTAINER_FULL;
    }
    mg_map_append(map, key, value);
    return 0;
}

inline const mg_value* mg_map_at(const mg_map* map, const char* key_str) {
    size_t key_size = strlen(key_str);
    if (key_size >= UINT32_MAX) {
        return NULL;
    }
    return mg_map_at2(map, (uint32_t)key_size, key_str);
}

inline const mg_value* mg_map_at2(const mg_map* map, uint32_t key_size,
    const char* key_data) {
    uint32_t pos = mg_map_find_key(map, key_size, key_data);
    if (pos != map->size) {
        return map->values[pos];
    }
    return NULL;
}

inline uint32_t mg_map_size(const mg_map* map) { return map->size; }

inline const mg_string* mg_map_key_at(const mg_map* map, uint32_t pos) {
    if (pos < map->size) {
        return map->keys[pos];
    } else {
        return NULL;
    }
}

inline const mg_value* mg_map_value_at(const mg_map* map, uint32_t pos) {
    if (pos < map->size) {
        return map->values[pos];
    } else {
        return NULL;
    }
}

inline mg_map* mg_map_copy_ca(const mg_map* map, mg_allocator* allocator) {
    if (!map) {
        return NULL;
    }
    mg_map* nmap = mg_map_alloc(map->size, allocator);
    if (!nmap) {
        return NULL;
    }
    nmap->capacity = map->size;
    nmap->size = map->size;
    uint32_t keys_copied = 0;
    uint32_t values_copied = 0;
    for (uint32_t i = 0; i < map->size; ++i) {
        nmap->keys[i] = mg_string_copy_ca(map->keys[i], allocator);
        if (!nmap->keys[i]) {
            goto cleanup;
        }
        keys_copied++;
        nmap->values[i] = mg_value_copy_ca(map->values[i], allocator);
        if (!nmap->values[i]) {
            goto cleanup;
        }
        values_copied++;
    }
    return nmap;

cleanup:
    for (uint32_t i = 0; i < keys_copied; ++i) {
        mg_string_destroy(map->keys[i]);
    }
    for (uint32_t i = 0; i < values_copied; ++i) {
        mg_value_destroy(map->values[i]);
    }
    mg_allocator_free(&mg_system_allocator, nmap);
    return NULL;
}

inline mg_map* mg_map_copy(const mg_map* map) {
    return mg_map_copy_ca(map, &mg_system_allocator);
}

inline void mg_map_destroy_ca(mg_map* map, mg_allocator* allocator) {
    if (!map || map == &mg_empty_map) {
        return;
    }
    for (uint32_t i = 0; i < map->size; ++i) {
        mg_string_destroy_ca(map->keys[i], allocator);
        mg_value_destroy_ca(map->values[i], allocator);
    }
    mg_allocator_free(allocator, map);
}

inline void mg_map_destroy(mg_map* map) {
    mg_map_destroy_ca(map, &mg_system_allocator);
}

inline int64_t mg_node_id(const mg_node* node) { return node->id; }

inline uint32_t mg_node_label_count(const mg_node* node) { return node->label_count; }

inline const mg_string* mg_node_label_at(const mg_node* node, uint32_t pos) {
    if (pos < node->label_count) {
        return node->labels[pos];
    } else {
        return NULL;
    }
}

inline const mg_map* mg_node_properties(const mg_node* node) {
    return node->properties;
}

inline mg_node* mg_node_copy_ca(const mg_node* node, mg_allocator* allocator) {
    if (!node) {
        return NULL;
    }
    mg_node* nnode = mg_node_alloc(node->label_count, &mg_system_allocator);
    if (!nnode) {
        return NULL;
    }
    nnode->id = node->id;
    nnode->label_count = 0;
    for (uint32_t i = 0; i < node->label_count; ++i) {
        nnode->labels[i] = mg_string_copy_ca(node->labels[i], allocator);
        if (!nnode->labels[i]) {
            goto cleanup;
        }
        nnode->label_count++;
    }
    nnode->properties = mg_map_copy_ca(node->properties, allocator);
    if (!nnode->properties) {
        goto cleanup;
    }
    return nnode;

cleanup:
    for (uint32_t i = 0; i < nnode->label_count; ++i) {
        mg_string_destroy(nnode->labels[i]);
    }
    mg_allocator_free(&mg_system_allocator, nnode);
    return NULL;
}

inline mg_node* mg_node_copy(const mg_node* node) {
    return mg_node_copy_ca(node, &mg_system_allocator);
}

inline void mg_node_destroy_ca(mg_node* node, mg_allocator* allocator) {
    if (!node) {
        return;
    }
    for (uint32_t i = 0; i < node->label_count; ++i) {
        mg_string_destroy_ca(node->labels[i], allocator);
    }
    mg_map_destroy_ca(node->properties, allocator);
    mg_allocator_free(allocator, node);
}

inline void mg_node_destroy(mg_node* node) {
    mg_node_destroy_ca(node, &mg_system_allocator);
}

inline int64_t mg_relationship_id(const mg_relationship* rel) { return rel->id; }

inline int64_t mg_relationship_start_id(const mg_relationship* rel) {
    return rel->start_id;
}

inline int64_t mg_relationship_end_id(const mg_relationship* rel) {
    return rel->end_id;
}

inline const mg_string* mg_relationship_type(const mg_relationship* rel) {
    return rel->type;
}

inline const mg_map* mg_relationship_properties(const mg_relationship* rel) {
    return rel->properties;
}

inline mg_relationship* mg_relationship_copy_ca(const mg_relationship* rel,
    mg_allocator* allocator) {
    if (!rel) {
        return NULL;
    }
    mg_relationship* nrel =
        (mg_relationship*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_relationship));
    if (!nrel) {
        return NULL;
    }
    nrel->id = rel->id;
    nrel->start_id = rel->start_id;
    nrel->end_id = rel->end_id;
    nrel->type = mg_string_copy_ca(rel->type, allocator);
    if (!nrel->type) {
        goto cleanup;
    }
    nrel->properties = mg_map_copy_ca(rel->properties, allocator);
    if (!nrel->properties) {
        goto cleanup_type;
    }
    return nrel;

cleanup_type:
    mg_string_destroy(nrel->type);

cleanup:
    mg_allocator_free(&mg_system_allocator, nrel);
    return NULL;
}

inline mg_relationship* mg_relationship_copy(const mg_relationship* rel) {
    return mg_relationship_copy_ca(rel, &mg_system_allocator);
}

inline void mg_relationship_destroy_ca(mg_relationship* rel, mg_allocator* allocator) {
    if (!rel) {
        return;
    }
    mg_string_destroy_ca(rel->type, allocator);
    mg_map_destroy_ca(rel->properties, allocator);
    mg_allocator_free(allocator, rel);
}

inline void mg_relationship_destroy(mg_relationship* rel) {
    mg_relationship_destroy_ca(rel, &mg_system_allocator);
}

inline int64_t mg_unbound_relationship_id(const mg_unbound_relationship* rel) {
    return rel->id;
}

inline const mg_string* mg_unbound_relationship_type(
    const mg_unbound_relationship* rel) {
    return rel->type;
}

inline const mg_map* mg_unbound_relationship_properties(
    const mg_unbound_relationship* rel) {
    return rel->properties;
}

inline mg_unbound_relationship* mg_unbound_relationship_copy_ca(
    const mg_unbound_relationship* rel, mg_allocator* allocator) {
    mg_unbound_relationship* nrel = (mg_unbound_relationship*)mg_allocator_malloc(
        &mg_system_allocator, sizeof(mg_unbound_relationship));
    if (!nrel) {
        return NULL;
    }
    nrel->id = rel->id;
    nrel->type = mg_string_copy_ca(rel->type, allocator);
    if (!nrel->type) {
        goto cleanup;
    }
    nrel->properties = mg_map_copy_ca(rel->properties, allocator);
    if (!nrel->properties) {
        goto cleanup_type;
    }
    return nrel;

cleanup_type:
    mg_string_destroy(nrel->type);

cleanup:
    mg_allocator_free(&mg_system_allocator, nrel);
    return NULL;
}

inline mg_unbound_relationship* mg_unbound_relationship_copy(
    const mg_unbound_relationship* rel) {
    return mg_unbound_relationship_copy_ca(rel, &mg_system_allocator);
}

inline void mg_unbound_relationship_destroy_ca(mg_unbound_relationship* rel,
    mg_allocator* allocator) {
    if (!rel) {
        return;
    }
    mg_string_destroy_ca(rel->type, allocator);
    mg_map_destroy_ca(rel->properties, allocator);
    mg_allocator_free(allocator, rel);
}

inline void mg_unbound_relationship_destroy(mg_unbound_relationship* rel) {
    mg_unbound_relationship_destroy_ca(rel, &mg_system_allocator);
}

inline uint32_t mg_path_length(const mg_path* path) {
    return path->sequence_length / 2;
}

inline const mg_node* mg_path_node_at(const mg_path* path, uint32_t pos) {
    if (pos <= path->sequence_length / 2) {
        if (pos == 0) {
            return path->nodes[0];
        }
        return path->nodes[path->sequence[2 * pos - 1]];
    } else {
        return NULL;
    }
}

inline const mg_unbound_relationship* mg_path_relationship_at(const mg_path* path,
    uint32_t pos) {
    if (pos < path->sequence_length / 2) {
        int64_t idx = path->sequence[2 * pos];
        if (idx < 0) {
            idx = -idx;
        }
        return path->relationships[idx - 1];
    } else {
        return NULL;
    }
}

inline int mg_path_relationship_reversed_at(const mg_path* path, uint32_t pos) {
    if (pos < path->sequence_length / 2) {
        int64_t idx = path->sequence[2 * pos];
        if (idx < 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

inline mg_path* mg_path_copy_ca(const mg_path* path, mg_allocator* allocator) {
    mg_path* npath = mg_path_alloc(path->node_count, path->relationship_count,
        path->sequence_length, &mg_system_allocator);
    if (!npath) {
        return NULL;
    }
    npath->node_count = 0;
    for (uint32_t i = 0; i < path->node_count; ++i) {
        npath->nodes[i] = mg_node_copy_ca(path->nodes[i], allocator);
        if (!npath->nodes[i]) {
            goto cleanup;
        }
        npath->node_count++;
    }
    npath->relationship_count = 0;
    for (uint32_t i = 0; i < path->relationship_count; ++i) {
        npath->relationships[i] =
            mg_unbound_relationship_copy_ca(path->relationships[i], allocator);
        if (!npath->relationships[i]) {
            goto cleanup;
        }
        npath->relationship_count++;
    }
    npath->sequence_length = path->sequence_length;
    memcpy(npath->sequence, path->sequence,
        path->sequence_length * sizeof(int64_t));
    return npath;

cleanup:
    for (uint32_t i = 0; i < npath->node_count; ++i) {
        mg_node_destroy(npath->nodes[i]);
    }
    for (uint32_t i = 0; i < npath->relationship_count; ++i) {
        mg_unbound_relationship_destroy(npath->relationships[i]);
    }
    mg_allocator_free(&mg_system_allocator, npath);
    return NULL;
}

inline mg_path* mg_path_copy(const mg_path* path) {
    return mg_path_copy_ca(path, &mg_system_allocator);
}

inline void mg_path_destroy_ca(mg_path* path, mg_allocator* allocator) {
    if (!path) {
        return;
    }
    for (uint32_t i = 0; i < path->node_count; ++i) {
        mg_node_destroy_ca(path->nodes[i], allocator);
    }
    for (uint32_t i = 0; i < path->relationship_count; ++i) {
        mg_unbound_relationship_destroy_ca(path->relationships[i], allocator);
    }
    mg_allocator_free(allocator, path);
}

inline void mg_path_destroy(mg_path* path) {
    mg_path_destroy_ca(path, &mg_system_allocator);
}

// SPATIAL AND TEMPORAL STRUCTURES
inline int64_t mg_date_days(const mg_date* date) { return date->days; }

inline int64_t mg_time_nanoseconds(const mg_time* time) { return time->nanoseconds; }

inline int64_t mg_time_tz_offset_seconds(const mg_time* time) {
    return time->tz_offset_seconds;
}

inline int64_t mg_local_time_nanoseconds(const mg_local_time* local_time) {
    return local_time->nanoseconds;
}

inline int64_t mg_date_time_seconds(const mg_date_time* date_time) {
    return date_time->seconds;
}

inline int64_t mg_date_time_nanoseconds(const mg_date_time* date_time) {
    return date_time->nanoseconds;
}

inline int64_t mg_date_time_tz_offset_minutes(const mg_date_time* date_time) {
    return date_time->tz_offset_minutes;
}

inline int64_t mg_date_time_zone_id_seconds(
    const mg_date_time_zone_id* date_time_zone_id) {
    return date_time_zone_id->seconds;
}

inline int64_t mg_date_time_zone_id_nanoseconds(
    const mg_date_time_zone_id* date_time_zone_id) {
    return date_time_zone_id->nanoseconds;
}

inline int64_t mg_date_time_zone_id_tz_id(
    const mg_date_time_zone_id* date_time_zone_id) {
    return date_time_zone_id->tz_id;
}

inline int64_t mg_local_date_time_seconds(const mg_local_date_time* local_date_time) {
    return local_date_time->seconds;
}

inline int64_t mg_local_date_time_nanoseconds(
    const mg_local_date_time* local_date_time) {
    return local_date_time->nanoseconds;
}

inline int64_t mg_duration_months(const mg_duration* duration) {
    return duration->months;
}

inline int64_t mg_duration_days(const mg_duration* duration) { return duration->days; }

inline int64_t mg_duration_seconds(const mg_duration* duration) {
    return duration->seconds;
}

inline int64_t mg_duration_nanoseconds(const mg_duration* duration) {
    return duration->nanoseconds;
}

inline int64_t mg_point_2d_srid(const mg_point_2d* point_2d) { return point_2d->srid; }

inline double mg_point_2d_x(const mg_point_2d* point_2d) { return point_2d->x; }

inline double mg_point_2d_y(const mg_point_2d* point_2d) { return point_2d->y; }

inline int64_t mg_point_3d_srid(const mg_point_3d* point_3d) { return point_3d->srid; }

inline double mg_point_3d_x(const mg_point_3d* point_3d) { return point_3d->x; }

inline double mg_point_3d_y(const mg_point_3d* point_3d) { return point_3d->y; }

inline double mg_point_3d_z(const mg_point_3d* point_3d) { return point_3d->z; }

inline mg_date* mg_date_copy_ca(const mg_date* src, mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_date* date = mg_date_alloc(allocator);
    if (!date) {
        return NULL;
    }
    memcpy(date, src, sizeof(mg_date));
    return date;
}

inline mg_date* mg_date_copy(const mg_date* date) {
    return mg_date_copy_ca(date, &mg_system_allocator);
}

inline void mg_date_destroy_ca(mg_date* date, mg_allocator* allocator) {
    if (!date) {
        return;
    }
    mg_allocator_free(allocator, date);
}

inline void mg_date_destroy(mg_date* date) {
    mg_date_destroy_ca(date, &mg_system_allocator);
}

inline mg_time* mg_time_copy_ca(const mg_time* src, mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_time* time = mg_time_alloc(allocator);
    if (!time) {
        return NULL;
    }
    memcpy(time, src, sizeof(mg_time));
    return time;
}

inline mg_time* mg_time_copy(const mg_time* time) {
    return mg_time_copy_ca(time, &mg_system_allocator);
}

inline void mg_time_destroy_ca(mg_time* time, mg_allocator* allocator) {
    if (!time) {
        return;
    }
    mg_allocator_free(allocator, time);
}

inline void mg_time_destroy(mg_time* time) {
    mg_time_destroy_ca(time, &mg_system_allocator);
}

inline mg_local_time* mg_local_time_copy_ca(const mg_local_time* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_local_time* local_time = mg_local_time_alloc(allocator);
    if (!local_time) {
        return NULL;
    }
    memcpy(local_time, src, sizeof(mg_local_time));
    return local_time;
}

inline mg_local_time* mg_local_time_copy(const mg_local_time* local_time) {
    return mg_local_time_copy_ca(local_time, &mg_system_allocator);
}

inline void mg_local_time_destroy_ca(mg_local_time* local_time,
    mg_allocator* allocator) {
    if (!local_time) {
        return;
    }
    mg_allocator_free(allocator, local_time);
}

inline void mg_local_time_destroy(mg_local_time* local_time) {
    mg_local_time_destroy_ca(local_time, &mg_system_allocator);
}

inline mg_date_time* mg_date_time_copy_ca(const mg_date_time* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_date_time* date_time = mg_date_time_alloc(allocator);
    if (!date_time) {
        return NULL;
    }
    memcpy(date_time, src, sizeof(mg_date_time));
    return date_time;
}

inline mg_date_time* mg_date_time_copy(const mg_date_time* date_time) {
    return mg_date_time_copy_ca(date_time, &mg_system_allocator);
}

inline void mg_date_time_destroy_ca(mg_date_time* date_time, mg_allocator* allocator) {
    if (!date_time) {
        return;
    }
    mg_allocator_free(allocator, date_time);
}

inline void mg_date_time_destroy(mg_date_time* date_time) {
    mg_date_time_destroy_ca(date_time, &mg_system_allocator);
}

inline mg_date_time_zone_id* mg_date_time_zone_id_copy_ca(
    const mg_date_time_zone_id* src, mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_date_time_zone_id* date_time_zone_id =
        mg_date_time_zone_id_alloc(allocator);
    if (!date_time_zone_id) {
        return NULL;
    }
    memcpy(date_time_zone_id, src, sizeof(mg_date_time_zone_id));
    return date_time_zone_id;
}

inline mg_date_time_zone_id* mg_date_time_zone_id_copy(
    const mg_date_time_zone_id* date_time_zone_id) {
    return mg_date_time_zone_id_copy_ca(date_time_zone_id, &mg_system_allocator);
}

inline void mg_date_time_zone_id_destroy_ca(mg_date_time_zone_id* date_time_zone_id,
    mg_allocator* allocator) {
    if (!date_time_zone_id) {
        return;
    }
    mg_allocator_free(allocator, date_time_zone_id);
}

inline void mg_date_time_zone_id_destroy(mg_date_time_zone_id* date_time_zone_id) {
    mg_date_time_zone_id_destroy_ca(date_time_zone_id, &mg_system_allocator);
}

inline mg_local_date_time* mg_local_date_time_copy_ca(const mg_local_date_time* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_local_date_time* local_date_time = mg_local_date_time_alloc(allocator);
    if (!local_date_time) {
        return NULL;
    }
    memcpy(local_date_time, src, sizeof(mg_local_date_time));
    return local_date_time;
}

inline mg_local_date_time* mg_local_date_time_copy(
    const mg_local_date_time* local_date_time) {
    return mg_local_date_time_copy_ca(local_date_time, &mg_system_allocator);
}

inline void mg_local_date_time_destroy_ca(mg_local_date_time* local_date_time,
    mg_allocator* allocator) {
    if (!local_date_time) {
        return;
    }
    mg_allocator_free(allocator, local_date_time);
}

inline void mg_local_date_time_destroy(mg_local_date_time* local_date_time) {
    mg_local_date_time_destroy_ca(local_date_time, &mg_system_allocator);
}

inline mg_duration* mg_duration_copy_ca(const mg_duration* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_duration* duration = mg_duration_alloc(allocator);
    if (!duration) {
        return NULL;
    }
    memcpy(duration, src, sizeof(mg_duration));
    return duration;
}

inline mg_duration* mg_duration_copy(const mg_duration* duration) {
    return mg_duration_copy_ca(duration, &mg_system_allocator);
}

inline void mg_duration_destroy_ca(mg_duration* duration, mg_allocator* allocator) {
    if (!duration) {
        return;
    }
    mg_allocator_free(allocator, duration);
}

inline void mg_duration_destroy(mg_duration* duration) {
    mg_duration_destroy_ca(duration, &mg_system_allocator);
}

inline mg_point_2d* mg_point_2d_copy_ca(const mg_point_2d* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_point_2d* point_2d = mg_point_2d_alloc(allocator);
    if (!point_2d) {
        return NULL;
    }
    memcpy(point_2d, src, sizeof(mg_point_2d));
    return point_2d;
}

inline mg_point_2d* mg_point_2d_copy(const mg_point_2d* point_2d) {
    return mg_point_2d_copy_ca(point_2d, &mg_system_allocator);
}

inline void mg_point_2d_destroy_ca(mg_point_2d* point_2d, mg_allocator* allocator) {
    if (!point_2d) {
        return;
    }
    mg_allocator_free(allocator, point_2d);
}

inline void mg_point_2d_destroy(mg_point_2d* point_2d) {
    mg_point_2d_destroy_ca(point_2d, &mg_system_allocator);
}

inline mg_point_3d* mg_point_3d_copy_ca(const mg_point_3d* src,
    mg_allocator* allocator) {
    if (!src) {
        return NULL;
    }
    mg_point_3d* point_3d = mg_point_3d_alloc(allocator);
    if (!point_3d) {
        return NULL;
    }
    memcpy(point_3d, src, sizeof(mg_point_3d));
    return point_3d;
}

inline mg_point_3d* mg_point_3d_copy(const mg_point_3d* point_3d) {
    return mg_point_3d_copy_ca(point_3d, &mg_system_allocator);
}

inline void mg_point_3d_destroy_ca(mg_point_3d* point_3d, mg_allocator* allocator) {
    if (!point_3d) {
        return;
    }
    mg_allocator_free(allocator, point_3d);
}

inline void mg_point_3d_destroy(mg_point_3d* point_3d) {
    mg_point_3d_destroy_ca(point_3d, &mg_system_allocator);
}

inline mg_node* mg_node_make(int64_t id, uint32_t label_count, mg_string** labels,
    mg_map* properties) {
    mg_node* node = mg_node_alloc(label_count, &mg_system_allocator);
    if (!node) {
        return NULL;
    }
    node->id = id;
    node->label_count = label_count;
    memcpy(node->labels, labels, label_count * sizeof(mg_string*));
    node->properties = properties;
    return node;
}

inline mg_relationship* mg_relationship_make(int64_t id, int64_t start_id,
    int64_t end_id, mg_string* type,
    mg_map* properties) {
    mg_relationship* rel =
        (mg_relationship*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_relationship));
    if (!rel) {
        return NULL;
    }
    rel->id = id;
    rel->start_id = start_id;
    rel->end_id = end_id;
    rel->type = type;
    rel->properties = properties;
    return rel;
}

inline mg_unbound_relationship* mg_unbound_relationship_make(int64_t id,
    mg_string* type,
    mg_map* properties) {
    mg_unbound_relationship* rel = (mg_unbound_relationship*)mg_allocator_malloc(
        &mg_system_allocator, sizeof(mg_unbound_relationship));
    if (!rel) {
        return NULL;
    }
    rel->id = id;
    rel->type = type;
    rel->properties = properties;
    return rel;
}

inline mg_path* mg_path_make(uint32_t node_count, mg_node** nodes,
    uint32_t relationship_count,
    mg_unbound_relationship** relationships,
    uint32_t sequence_length, const int64_t* const sequence) {
    mg_path* path = mg_path_alloc(node_count, relationship_count, sequence_length,
        &mg_system_allocator);
    if (!path) {
        return NULL;
    }
    path->node_count = node_count;
    memcpy(path->nodes, nodes, node_count * sizeof(mg_node*));
    path->relationship_count = relationship_count;
    memcpy(path->relationships, relationships,
        relationship_count * sizeof(mg_unbound_relationship*));
    path->sequence_length = sequence_length;
    memcpy(path->sequence, sequence, sequence_length * sizeof(int64_t));
    return path;
}

inline mg_date* mg_date_make(int64_t days) {
    mg_date* date = mg_date_alloc(&mg_system_allocator);
    if (!date) {
        return NULL;
    }
    date->days = days;
    return date;
}

inline mg_local_time* mg_local_time_make(int64_t nanoseconds) {
    mg_local_time* lt = mg_local_time_alloc(&mg_system_allocator);
    if (!lt) {
        return NULL;
    }
    lt->nanoseconds = nanoseconds;
    return lt;
}

inline mg_local_date_time* mg_local_date_time_make(int64_t seconds,
    int64_t nanoseconds) {
    mg_local_date_time* ldt = mg_local_date_time_alloc(&mg_system_allocator);
    if (!ldt) {
        return NULL;
    }
    ldt->seconds = seconds;
    ldt->nanoseconds = nanoseconds;
    return ldt;
}

inline mg_duration* mg_duration_make(int64_t months, int64_t days, int64_t seconds,
    int64_t nanoseconds) {
    mg_duration* dur = mg_duration_alloc(&mg_system_allocator);
    if (!dur) {
        return NULL;
    }
    dur->months = months;
    dur->days = days;
    dur->seconds = seconds;
    dur->nanoseconds = nanoseconds;
    return dur;
}

inline int mg_string_equal(const mg_string* lhs, const mg_string* rhs) {
    if (lhs->size != rhs->size) {
        return 0;
    }
    return memcmp(lhs->data, rhs->data, lhs->size) == 0;
}

inline int mg_map_equal(const mg_map* lhs, const mg_map* rhs) {
    if (lhs->size != rhs->size) {
        return 0;
    }
    for (uint32_t i = 0; i < lhs->size; ++i) {
        if (!mg_string_equal(lhs->keys[i], rhs->keys[i])) return 0;
        if (!mg_value_equal(lhs->values[i], rhs->values[i])) return 0;
    }
    return 1;
}

inline int mg_node_equal(const mg_node* lhs, const mg_node* rhs) {
    if (lhs->id != rhs->id) {
        return 0;
    }
    if (lhs->label_count != rhs->label_count) {
        return 0;
    }
    for (uint32_t i = 0; i < lhs->label_count; ++i) {
        if (!mg_string_equal(lhs->labels[i], rhs->labels[i])) {
            return 0;
        }
    }
    return mg_map_equal(lhs->properties, rhs->properties);
}

inline int mg_relationship_equal(const mg_relationship* lhs,
    const mg_relationship* rhs) {
    if (lhs->id != rhs->id || lhs->start_id != rhs->start_id ||
        lhs->end_id != rhs->end_id) {
        return 0;
    }
    if (!mg_string_equal(lhs->type, rhs->type)) {
        return 0;
    }
    return mg_map_equal(lhs->properties, rhs->properties);
}

inline int mg_unbound_relationship_equal(const mg_unbound_relationship* lhs,
    const mg_unbound_relationship* rhs) {
    if (lhs->id != rhs->id) {
        return 0;
    }
    if (!mg_string_equal(lhs->type, rhs->type)) {
        return 0;
    }
    return mg_map_equal(lhs->properties, rhs->properties);
}

inline int mg_path_equal(const mg_path* lhs, const mg_path* rhs) {
    if (lhs->node_count != rhs->node_count ||
        lhs->relationship_count != rhs->relationship_count ||
        lhs->sequence_length != rhs->sequence_length) {
        return 0;
    }
    for (uint32_t i = 0; i < lhs->node_count; ++i) {
        if (!mg_node_equal(lhs->nodes[i], rhs->nodes[i])) {
            return 0;
        }
    }
    for (uint32_t i = 0; i < lhs->relationship_count; ++i) {
        if (!mg_unbound_relationship_equal(lhs->relationships[i],
            rhs->relationships[i])) {
            return 0;
        }
    }
    for (uint32_t i = 0; i < lhs->sequence_length; ++i) {
        if (lhs->sequence[i] != rhs->sequence[i]) {
            return 0;
        }
    }
    return 1;
}

inline int mg_date_equal(const mg_date* lhs, const mg_date* rhs) {
    return lhs->days == rhs->days;
}

inline int mg_time_equal(const mg_time* lhs, const mg_time* rhs) {
    return lhs->nanoseconds == rhs->nanoseconds &&
        lhs->tz_offset_seconds == rhs->tz_offset_seconds;
}

inline int mg_local_time_equal(const mg_local_time* lhs, const mg_local_time* rhs) {
    return lhs->nanoseconds == rhs->nanoseconds;
}

inline int mg_date_time_equal(const mg_date_time* lhs, const mg_date_time* rhs) {
    return lhs->seconds == rhs->seconds && lhs->nanoseconds == rhs->nanoseconds &&
        lhs->tz_offset_minutes == rhs->tz_offset_minutes;
}

inline int mg_local_date_time_equal(const mg_local_date_time* lhs,
    const mg_local_date_time* rhs) {
    return lhs->seconds == rhs->seconds && lhs->nanoseconds == rhs->nanoseconds;
}

inline int mg_date_time_zone_id_equal(const mg_date_time_zone_id* lhs,
    const mg_date_time_zone_id* rhs) {
    return lhs->seconds == rhs->seconds && lhs->nanoseconds == rhs->nanoseconds &&
        lhs->tz_id == rhs->tz_id;
}

inline int mg_duration_equal(const mg_duration* lhs, const mg_duration* rhs) {
    return lhs->days == rhs->days && lhs->months == rhs->months &&
        lhs->seconds == rhs->seconds && lhs->nanoseconds == rhs->nanoseconds;
}

inline int mg_point_2d_equal(const mg_point_2d* lhs, const mg_point_2d* rhs) {
    return lhs->srid == rhs->srid && lhs->x == rhs->x && lhs->y == rhs->y;
}

inline int mg_point_3d_equal(const mg_point_3d* lhs, const mg_point_3d* rhs) {
    return lhs->srid == rhs->srid && lhs->x == rhs->x && lhs->y == rhs->y &&
        lhs->z == rhs->z;
}

inline int mg_value_equal(const mg_value* lhs, const mg_value* rhs) {
    if (lhs->type != rhs->type) {
        return 0;
    }
    switch (lhs->type) {
        case MG_VALUE_TYPE_NULL:
            return 1;

        case MG_VALUE_TYPE_BOOL:
            return (lhs->bool_v == 0) == (rhs->bool_v == 0);

        case MG_VALUE_TYPE_INTEGER:
            return lhs->integer_v == rhs->integer_v;

        case MG_VALUE_TYPE_FLOAT:
            return lhs->integer_v == rhs->integer_v;

        case MG_VALUE_TYPE_STRING:
            return mg_string_equal(lhs->string_v, rhs->string_v);
        case MG_VALUE_TYPE_LIST:
            if (lhs->list_v->size != rhs->list_v->size) {
                return 0;
            }
            for (uint32_t i = 0; i < lhs->list_v->size; ++i) {
                if (!mg_value_equal(lhs->list_v->elements[i], rhs->list_v->elements[i]))
                    return 0;
            }
            return 1;
        case MG_VALUE_TYPE_MAP:
            return mg_map_equal(lhs->map_v, rhs->map_v);
        case MG_VALUE_TYPE_NODE:
            return mg_node_equal(lhs->node_v, rhs->node_v);
        case MG_VALUE_TYPE_RELATIONSHIP:
            return mg_relationship_equal(lhs->relationship_v, rhs->relationship_v);
        case MG_VALUE_TYPE_UNBOUND_RELATIONSHIP:
            return mg_unbound_relationship_equal(lhs->unbound_relationship_v,
                rhs->unbound_relationship_v);
        case MG_VALUE_TYPE_PATH:
            return mg_path_equal(lhs->path_v, rhs->path_v);
        case MG_VALUE_TYPE_DATE:
            return mg_date_equal(lhs->date_v, rhs->date_v);
        case MG_VALUE_TYPE_TIME:
            return mg_time_equal(lhs->time_v, rhs->time_v);
        case MG_VALUE_TYPE_LOCAL_TIME:
            return mg_local_time_equal(lhs->local_time_v, rhs->local_time_v);
        case MG_VALUE_TYPE_DATE_TIME:
            return mg_date_time_equal(lhs->date_time_v, rhs->date_time_v);
        case MG_VALUE_TYPE_DATE_TIME_ZONE_ID:
            return mg_date_time_zone_id_equal(lhs->date_time_zone_id_v,
                rhs->date_time_zone_id_v);
        case MG_VALUE_TYPE_LOCAL_DATE_TIME:
            return mg_local_date_time_equal(lhs->local_date_time_v,
                rhs->local_date_time_v);
        case MG_VALUE_TYPE_DURATION:
            return mg_duration_equal(lhs->duration_v, rhs->duration_v);
        case MG_VALUE_TYPE_POINT_2D:
            return mg_point_2d_equal(lhs->point_2d_v, rhs->point_2d_v);
        case MG_VALUE_TYPE_POINT_3D:
            return mg_point_3d_equal(lhs->point_3d_v, rhs->point_3d_v);
        case MG_VALUE_TYPE_UNKNOWN:
            return 0;
    }
    return 0;
}


//mgmessage.c-------------------------------------------------------------------------

inline void mg_message_success_destroy_ca(mg_message_success* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_map_destroy_ca(message->metadata, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_failure_destroy_ca(mg_message_failure* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_map_destroy_ca(message->metadata, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_record_destroy_ca(mg_message_record* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_list_destroy_ca(message->fields, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_init_destroy_ca(mg_message_init* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_string_destroy_ca(message->client_name, allocator);
    mg_map_destroy_ca(message->auth_token, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_hello_destroy_ca(mg_message_hello* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_map_destroy_ca(message->extra, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_run_destroy_ca(mg_message_run* message,
    mg_allocator* allocator) {
    if (!message) return;
    mg_string_destroy_ca(message->statement, allocator);
    mg_map_destroy_ca(message->parameters, allocator);
    mg_map_destroy_ca(message->extra, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_begin_destroy_ca(mg_message_begin* message,
    mg_allocator* allocator) {
    if (!message) {
        return;
    }
    mg_map_destroy_ca(message->extra, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_pull_destroy_ca(mg_message_pull* message,
    mg_allocator* allocator) {
    if (!message) {
        return;
    }
    mg_map_destroy_ca(message->extra, allocator);
    mg_allocator_free(allocator, message);
}

inline void mg_message_destroy_ca(mg_message* message, mg_allocator* allocator) {
    if (!message) return;
    switch (message->type) {
        case MG_MESSAGE_TYPE_SUCCESS:
            mg_message_success_destroy_ca(message->success_v, allocator);
            break;
        case MG_MESSAGE_TYPE_FAILURE:
            mg_message_failure_destroy_ca(message->failure_v, allocator);
            break;
        case MG_MESSAGE_TYPE_RECORD:
            mg_message_record_destroy_ca(message->record_v, allocator);
            break;
        case MG_MESSAGE_TYPE_INIT:
            mg_message_init_destroy_ca(message->init_v, allocator);
            break;
        case MG_MESSAGE_TYPE_HELLO:
            mg_message_hello_destroy_ca(message->hello_v, allocator);
            break;
        case MG_MESSAGE_TYPE_RUN:
            mg_message_run_destroy_ca(message->run_v, allocator);
            break;
        case MG_MESSAGE_TYPE_BEGIN:
            mg_message_begin_destroy_ca(message->begin_v, allocator);
            break;
        case MG_MESSAGE_TYPE_PULL:
            mg_message_pull_destroy_ca(message->pull_v, allocator);
            break;
        case MG_MESSAGE_TYPE_ACK_FAILURE:
        case MG_MESSAGE_TYPE_RESET:
        case MG_MESSAGE_TYPE_COMMIT:
        case MG_MESSAGE_TYPE_ROLLBACK:
            break;
    }
    mg_allocator_free(allocator, message);
}

//mgsession.c-------------------------------------------------------------------------

inline int mg_session_status(const mg_session* session) {
    if (!session) {
        return MG_SESSION_BAD;
    }
    return session->status;
}

#define MG_DECODER_ALLOCATOR_BLOCK_SIZE 131072
// This seems like a reasonable value --- at most 4 kB per block is wasted
// (around 3% of allocated memory, not including padding), and separate
// allocations should only happen for really big objects (lists with more than
// 500 elements, maps with more than 250 elements, strings with more than 4000
// characters, ...).
#define MG_DECODER_SEP_ALLOC_THRESHOLD 4096

inline mg_session* mg_session_init(mg_allocator* allocator) {
    mg_linear_allocator* decoder_allocator =
        mg_linear_allocator_init(allocator, MG_DECODER_ALLOCATOR_BLOCK_SIZE,
            MG_DECODER_SEP_ALLOC_THRESHOLD);
    if (!decoder_allocator) {
        return NULL;
    }

    mg_session* session = (mg_session*)mg_allocator_malloc(allocator, sizeof(mg_session));
    if (!session) {
        mg_linear_allocator_destroy(decoder_allocator);
        return NULL;
    }

    session->transport = NULL;
    session->allocator = allocator;
    session->decoder_allocator = (mg_allocator*)decoder_allocator;
    session->out_buffer = NULL;
    session->in_buffer = NULL;
    session->out_capacity = MG_BOLT_CHUNK_HEADER_SIZE + MG_BOLT_MAX_CHUNK_SIZE;
    session->out_buffer = (char*)mg_allocator_malloc(allocator, session->out_capacity);
    if (!session->out_buffer) {
        goto cleanup;
    }
    session->out_begin = MG_BOLT_CHUNK_HEADER_SIZE;
    session->out_end = session->out_begin;

    session->in_capacity = MG_BOLT_MAX_CHUNK_SIZE;
    session->in_buffer = (char*)mg_allocator_malloc(allocator, session->in_capacity);
    if (!session->in_buffer) {
        goto cleanup;
    }
    session->in_end = 0;
    session->in_cursor = 0;

    session->result.session = session;
    session->result.message = NULL;
    session->result.columns = NULL;

    session->explicit_transaction = 0;
    session->query_number = 0;

    session->error_buffer[0] = 0;

    return session;

cleanup:
    mg_linear_allocator_destroy(decoder_allocator);
    mg_allocator_free(allocator, session->in_buffer);
    mg_allocator_free(allocator, session->out_buffer);
    mg_allocator_free(allocator, session);
    return NULL;
}

inline void mg_session_set_error(mg_session* session, const char* fmt, ...) {
    va_list arglist;
    va_start(arglist, fmt);
    if (vsnprintf(session->error_buffer, MG_MAX_ERROR_SIZE, fmt, arglist) < 0) {
        strncpy(session->error_buffer, "couldn't set error message",
            MG_MAX_ERROR_SIZE);
    }
    va_end(arglist);
}

inline const char* mg_session_error(mg_session* session) {
    if (!session) {
        return "session is NULL (possibly out of memory)";
    }
    return session->error_buffer;
}

inline void mg_session_invalidate(mg_session* session) {
    if (session->transport) {
        mg_transport_destroy(session->transport);
        session->transport = NULL;
    }
    session->status = MG_SESSION_BAD;
}

inline void mg_session_destroy(mg_session* session) {
    if (!session) {
        return;
    }
    if (session->transport) {
        mg_transport_destroy(session->transport);
    }
    mg_allocator_free(session->allocator, session->in_buffer);
    mg_allocator_free(session->allocator, session->out_buffer);

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;
    mg_list_destroy_ca(session->result.columns, session->allocator);
    session->result.columns = NULL;

    mg_linear_allocator_destroy(
        (mg_linear_allocator*)session->decoder_allocator);
    mg_allocator_free(session->allocator, session);
}

inline int mg_session_flush_chunk(mg_session* session) {
    size_t chunk_size = session->out_end - session->out_begin;
    if (!chunk_size) {
        return 0;
    }
    if (chunk_size > MG_BOLT_MAX_CHUNK_SIZE) {
        abort();
    }

    // Actual chunk data is written with offset of two bytes, leaving 2 bytes for
    // chunk size which we write here before sending.
    assert(session->out_begin == MG_BOLT_CHUNK_HEADER_SIZE);
    assert(MG_BOLT_CHUNK_HEADER_SIZE == sizeof(uint16_t));

    *(uint16_t*)session->out_buffer = htobe16((uint16_t)chunk_size);

    if (mg_transport_send(session->transport, session->out_buffer,
        session->out_end) != 0) {
        mg_session_set_error(session, "failed to send chunk data");
        return MG_ERROR_SEND_FAILED;
    }

    session->out_end = session->out_begin;
    return 0;
}

inline int mg_session_flush_message(mg_session* session) {
    {
        int status = mg_session_flush_chunk(session);
        if (status != 0) {
            return status;
        }
    }
    const char MESSAGE_END[] = { 0x00, 0x00 };
    {
        int status =
            mg_transport_send(session->transport, MESSAGE_END, sizeof(MESSAGE_END));
        if (status != 0) {
            mg_session_set_error(session, "failed to send message end marker");
            return MG_ERROR_SEND_FAILED;
        }
    }
    return 0;
}

inline int mg_session_write_raw(mg_session* session, const char* data, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        size_t buffer_free = session->out_capacity - session->out_end;
        if (len - sent >= buffer_free) {
            memcpy(session->out_buffer + session->out_end, data + sent, buffer_free);
            session->out_end = session->out_capacity;
            sent += buffer_free;
            {
                int status = mg_session_flush_chunk(session);
                if (status != 0) {
                    return status;
                }
            }
        } else {
            memcpy(session->out_buffer + session->out_end, data + sent, len - sent);
            session->out_end += len - sent;
            sent = len;
        }
    }
    return 0;
}

inline int mg_session_ensure_space_for_chunk(mg_session* session, size_t chunk_size) {
    while (session->in_capacity - session->in_end < chunk_size) {
        char* new_in_buffer = (char*)mg_allocator_realloc(
            session->allocator, session->in_buffer, 2 * session->in_capacity);
        if (!new_in_buffer) {
            mg_session_set_error(session,
                "failed to enlarge incoming message buffer");
            return MG_ERROR_OOM;
        }
        session->in_capacity = 2 * session->in_capacity;
        session->in_buffer = new_in_buffer;
    }
    return 0;
}

inline int mg_session_read_chunk(mg_session* session) {
    uint16_t chunk_size;
    mg_transport_suspend_until_ready_to_read(session->transport);
    if (mg_transport_recv(session->transport, (char*)&chunk_size, 2) != 0) {
        mg_session_set_error(session, "failed to receive chunk size");
        return MG_ERROR_RECV_FAILED;
    }
    chunk_size = be16toh(chunk_size);
    if (chunk_size == 0) {
        return 0;
    }
    {
        int status = mg_session_ensure_space_for_chunk(session, chunk_size);
        if (status != 0) {
            return status;
        }
    }
    mg_transport_suspend_until_ready_to_read(session->transport);
    if (mg_transport_recv(session->transport,
        session->in_buffer + session->in_end,
        chunk_size) != 0) {
        mg_session_set_error(session, "failed to receive chunk data");
        return MG_ERROR_RECV_FAILED;
    }
    session->in_end += chunk_size;
    return 1;
}

inline int mg_session_receive_message(mg_session* session) {
    // At this point, we reset the session decoder allocator and all objects from
    // the previous message are lost.
    mg_linear_allocator_reset((mg_linear_allocator*)session->decoder_allocator);
    session->in_end = 0;
    session->in_cursor = 0;
    int status;
    do {
        status = mg_session_read_chunk(session);
    } while (status == 1);
    return status;
}

//mgsession-decoder.c-------------------------------------------------------------------------

inline int mg_session_read_uint8(mg_session* session, uint8_t* val) {
    if (session->in_cursor + 1 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    *val = *(uint8_t*)(session->in_buffer + session->in_cursor);
    session->in_cursor += 1;
    return 0;
}

inline int mg_session_read_uint16(mg_session* session, uint16_t* val) {
    if (session->in_cursor + 2 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    *val = be16toh(*(uint16_t*)(session->in_buffer + session->in_cursor));
    session->in_cursor += 2;
    return 0;
}

inline int mg_session_read_uint32(mg_session* session, uint32_t* val) {
    if (session->in_cursor + 4 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    *val = be32toh(*(uint32_t*)(session->in_buffer + session->in_cursor));
    session->in_cursor += 4;
    return 0;
}

inline int mg_session_read_uint64(mg_session* session, uint64_t* val) {
    if (session->in_cursor + 8 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    *val = be64toh(*(uint64_t*)(session->in_buffer + session->in_cursor));
    session->in_cursor += 8;
    return 0;
}

inline int mg_session_read_null(mg_session* session) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));
    if (marker != MG_MARKER_NULL) {
        mg_session_set_error(session, "wrong value marker");
        return MG_ERROR_DECODING_FAILED;
    }
    return 0;
}

inline int mg_session_read_integer(mg_session* session, int64_t* val) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));

    if ((marker & 0x80) == 0) {
        *val = marker;
        return 0;
    }

    if ((marker & 0xF0) == 0xF0) {
        *val = (int64_t)marker - 256;
        return 0;
    }

    switch (marker) {
        case MG_MARKER_INT_8: {
            uint8_t tmp;
            MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &tmp));
            *val = *(int8_t*)&tmp;
            return 0;
        }
        case MG_MARKER_INT_16: {
            uint16_t tmp;
            MG_RETURN_IF_FAILED(mg_session_read_uint16(session, &tmp));
            *val = *(int16_t*)&tmp;
            return 0;
        }
        case MG_MARKER_INT_32: {
            uint32_t tmp;
            MG_RETURN_IF_FAILED(mg_session_read_uint32(session, &tmp));
            *val = *(int32_t*)&tmp;
            return 0;
        }
        case MG_MARKER_INT_64: {
            uint64_t tmp;
            MG_RETURN_IF_FAILED(mg_session_read_uint64(session, &tmp));
            *val = *(int64_t*)&tmp;
            return 0;
        }
    }

    mg_session_set_error(session, "wrong value marker");
    return MG_ERROR_DECODING_FAILED;
}

inline int mg_session_read_bool(mg_session* session, int* value) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));

    switch (marker) {
        case MG_MARKER_BOOL_FALSE:
            *value = 0;
            return 0;
        case MG_MARKER_BOOL_TRUE:
            *value = 1;
            return 0;
    }

    mg_session_set_error(session, "wrong value marker");
    return MG_ERROR_DECODING_FAILED;
}

inline int mg_session_read_float(mg_session* session, double* value) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));
    if (marker != MG_MARKER_FLOAT) {
        mg_session_set_error(session, "wrong value marker");
        return MG_ERROR_DECODING_FAILED;
    }
    uint64_t as_uint64;
    MG_RETURN_IF_FAILED(mg_session_read_uint64(session, &as_uint64));
    memcpy(value, &as_uint64, sizeof(double));
    return 0;
}

/// Markers have to be ordered from smallest to largest.
inline int mg_session_read_container_size(mg_session* session, uint32_t* size,
    const uint8_t* markers) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));

    if ((marker & 0xF0) == markers[0]) {
        *size = marker & 0xF;
        return 0;
    }

    if (marker == markers[1]) {
        uint8_t tmp;
        MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &tmp));
        *size = tmp;
    } else if (marker == markers[2]) {
        uint16_t tmp;
        MG_RETURN_IF_FAILED(mg_session_read_uint16(session, &tmp));
        *size = tmp;
    } else if (marker == markers[3]) {
        uint32_t tmp;
        MG_RETURN_IF_FAILED(mg_session_read_uint32(session, &tmp));
        *size = tmp;
    } else {
        mg_session_set_error(session, "wrong value marker");
        return MG_ERROR_DECODING_FAILED;
    }

    return 0;
}

inline int mg_session_read_string(mg_session* session, mg_string** str) {
    uint32_t size;
    MG_RETURN_IF_FAILED(
        mg_session_read_container_size(session, &size, MG_MARKERS_STRING));

    if (session->in_cursor + size > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }

    mg_string* tstr = mg_string_alloc(size, session->decoder_allocator);
    if (!tstr) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    tstr->size = size;
    memcpy(tstr->data, session->in_buffer + session->in_cursor, size);
    session->in_cursor += size;
    *str = tstr;
    return 0;
}

inline int mg_session_read_list(mg_session* session, mg_list** list) {
    uint32_t size;
    MG_RETURN_IF_FAILED(
        mg_session_read_container_size(session, &size, MG_MARKERS_LIST));

    mg_list* tlist = mg_list_alloc(size, session->decoder_allocator);
    if (!tlist) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    tlist->size = 0;
    for (uint32_t i = 0; i < size; ++i) {
        status = mg_session_read_value(session, &tlist->elements[i]);
        if (status != 0) {
            goto cleanup;
        }
        tlist->size++;
    }

    *list = tlist;
    return 0;

cleanup:
    for (uint32_t i = 0; i < tlist->size; ++i) {
        mg_value_destroy_ca(tlist->elements[i], session->decoder_allocator);
    }
    mg_allocator_free(session->decoder_allocator, tlist);
    return status;
}

inline int mg_session_read_map(mg_session* session, mg_map** map) {
    uint32_t size;
    MG_RETURN_IF_FAILED(
        mg_session_read_container_size(session, &size, MG_MARKERS_MAP));

    mg_map* tmap = mg_map_alloc(size, session->decoder_allocator);
    if (!tmap) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    tmap->size = size;

    uint32_t keys_read = 0;
    uint32_t values_read = 0;
    for (uint32_t i = 0; i < size; ++i) {
        status = mg_session_read_string(session, &tmap->keys[i]);
        if (status != 0) {
            goto cleanup;
        }
        keys_read++;
        status = mg_session_read_value(session, &tmap->values[i]);
        if (status != 0) {
            goto cleanup;
        }
        values_read++;
    }

    *map = tmap;
    return 0;

cleanup:
    for (uint32_t i = 0; i < keys_read; ++i) {
        mg_string_destroy_ca(tmap->keys[i], session->decoder_allocator);
    }
    for (uint32_t i = 0; i < values_read; ++i) {
        mg_value_destroy_ca(tmap->values[i], session->decoder_allocator);
    }
    mg_allocator_free(session->decoder_allocator, tmap);
    return status;
}

inline int mg_session_check_struct_header(mg_session* session, uint8_t marker,
    uint8_t signature) {
    if (session->in_cursor + 2 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    uint8_t* header = (uint8_t*)(session->in_buffer + session->in_cursor);
    if (header[0] != marker) {
        mg_session_set_error(session, "wrong value marker");
        return MG_ERROR_DECODING_FAILED;
    }
    if (header[1] != signature) {
        mg_session_set_error(session, "wrong struct signature");
        return MG_ERROR_DECODING_FAILED;
    }
    session->in_cursor += 2;
    return 0;
}

inline int mg_session_read_node(mg_session* session, mg_node** node) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3), MG_SIGNATURE_NODE));

    int64_t id;
    MG_RETURN_IF_FAILED(mg_session_read_integer(session, &id));

    uint32_t label_count;
    MG_RETURN_IF_FAILED(
        mg_session_read_container_size(session, &label_count, MG_MARKERS_LIST));

    mg_node* tnode = mg_node_alloc(label_count, session->decoder_allocator);
    if (!tnode) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    tnode->id = id;
    tnode->label_count = 0;
    for (uint32_t i = 0; i < label_count; ++i) {
        status = mg_session_read_string(session, &tnode->labels[i]);
        if (status != 0) {
            goto cleanup;
        }
        tnode->label_count++;
    }

    status = mg_session_read_map(session, &tnode->properties);
    if (status != 0) {
        goto cleanup;
    }

    *node = tnode;

    return 0;

cleanup:
    for (uint32_t i = 0; i < tnode->label_count; ++i) {
        mg_string_destroy_ca(tnode->labels[i], session->decoder_allocator);
    }
    mg_allocator_free(session->decoder_allocator, tnode);
    return status;
}

inline int mg_session_read_relationship(mg_session* session, mg_relationship** rel) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 5),
        MG_SIGNATURE_RELATIONSHIP));

    mg_relationship* trel =
        (mg_relationship*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_relationship));
    if (!trel) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    status = mg_session_read_integer(session, &trel->id);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &trel->start_id);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &trel->end_id);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_string(session, &trel->type);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_map(session, &trel->properties);
    if (status != 0) {
        goto cleanup_type;
    }

    *rel = trel;
    return 0;

cleanup_type:
    mg_string_destroy_ca(trel->type, session->decoder_allocator);

cleanup:
    mg_allocator_free(session->decoder_allocator, trel);
    return status;
}

inline int mg_session_read_unbound_relationship(mg_session* session,
    mg_unbound_relationship** rel) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3),
        MG_SIGNATURE_UNBOUND_RELATIONSHIP));

    mg_unbound_relationship* trel = (mg_unbound_relationship*)mg_allocator_malloc(
        session->decoder_allocator, sizeof(mg_unbound_relationship));
    if (!trel) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    status = mg_session_read_integer(session, &trel->id);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_string(session, &trel->type);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_map(session, &trel->properties);
    if (status != 0) {
        goto cleanup_type;
    }

    *rel = trel;
    return 0;

cleanup_type:
    mg_string_destroy_ca(trel->type, session->decoder_allocator);

cleanup:
    mg_allocator_free(session->decoder_allocator, trel);
    return status;
}

inline int mg_session_read_path(mg_session* session, mg_path** path) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3), MG_SIGNATURE_PATH));

    uint32_t node_count;
    MG_RETURN_IF_FAILED(
        mg_session_read_container_size(session, &node_count, MG_MARKERS_LIST));

    // There must be at least one node in the node list.
    if (!node_count) {
        mg_session_set_error(session, "invalid path: empty node list");
        return MG_ERROR_DECODING_FAILED;
    }

    mg_node** nodes = (mg_node**)mg_allocator_malloc(session->decoder_allocator,
        node_count * sizeof(mg_node*));
    if (!nodes) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    uint32_t nodes_read = 0;
    mg_unbound_relationship** relationships;
    uint32_t relationships_read;
    int64_t* sequence;
    mg_path* tpath;
    for (uint32_t i = 0; i < node_count; ++i) {
        status = mg_session_read_node(session, &nodes[i]);
        if (status != 0) {
            goto cleanup_nodes;
        }
        nodes_read++;
    }

    uint32_t relationship_count;
    status = mg_session_read_container_size(session, &relationship_count,
        MG_MARKERS_LIST);
    if (status != 0) {
        goto cleanup_nodes;
    }

    relationships = (mg_unbound_relationship**)mg_allocator_malloc(
        session->decoder_allocator,
        relationship_count * sizeof(mg_unbound_relationship*));
    if (!relationships) {
        mg_session_set_error(session, "out of memory");
        status = MG_ERROR_OOM;
        goto cleanup_nodes;
    }

    relationships_read = 0;
    for (uint32_t i = 0; i < relationship_count; ++i) {
        status = mg_session_read_unbound_relationship(session, &relationships[i]);
        if (status != 0) {
            goto cleanup_relationships;
        }
        relationships_read++;
    }

    uint32_t sequence_length;
    status = mg_session_read_container_size(session, &sequence_length,
        MG_MARKERS_LIST);
    if (status != 0) {
        goto cleanup_relationships;
    }
    // Path is an alternating sequence of nodes and relationships starting and
    // ending with a node, so it's length must be odd. First node on the path is
    // implicit so sequence length must be even.
    if (sequence_length % 2 != 0) {
        mg_session_set_error(session, "invalid path: odd sequence length");
        status = MG_ERROR_DECODING_FAILED;
        goto cleanup_relationships;
    }

    sequence = (int64_t*)mg_allocator_malloc(session->decoder_allocator,
        sequence_length * sizeof(int64_t));
    if (!sequence) {
        mg_session_set_error(session, "out of memory");
        status = MG_ERROR_OOM;
        goto cleanup_relationships;
    }

    for (uint32_t i = 0; i < sequence_length; ++i) {
        status = mg_session_read_integer(session, &sequence[i]);
        if (status != 0) {
            goto cleanup_sequence;
        }
        if (i % 2 == 0) {
            // Relationships are 1-indexed with sign determining direction.
            int64_t idx = sequence[i] > 0 ? sequence[i] : -sequence[i];
            if (idx < 1 || idx > relationship_count) {
                mg_session_set_error(session,
                    "invalid path: relationship index out of range");
                status = MG_ERROR_DECODING_FAILED;
                goto cleanup_sequence;
            }
        } else {
            // Nodes are 0-indexed.
            if (sequence[i] < 0 || sequence[i] >= node_count) {
                mg_session_set_error(session, "invalid path: node index out of range");
                status = MG_ERROR_DECODING_FAILED;
                goto cleanup_sequence;
            }
        }
    }

    tpath = mg_path_alloc(node_count, relationship_count,
        sequence_length, session->decoder_allocator);
    if (!tpath) {
        mg_session_set_error(session, "out of memory");
        status = MG_ERROR_OOM;
        goto cleanup_sequence;
    }

    tpath->node_count = node_count;
    memcpy(tpath->nodes, nodes, node_count * sizeof(mg_node*));
    mg_allocator_free(session->decoder_allocator, nodes);

    tpath->relationship_count = relationship_count;
    memcpy(tpath->relationships, relationships,
        relationship_count * sizeof(mg_unbound_relationship*));
    mg_allocator_free(session->decoder_allocator, relationships);

    tpath->sequence_length = sequence_length;
    memcpy(tpath->sequence, sequence, sequence_length * sizeof(int64_t));
    mg_allocator_free(session->decoder_allocator, sequence);

    *path = tpath;
    return 0;

cleanup_sequence:
    mg_allocator_free(session->decoder_allocator, sequence);

cleanup_relationships:
    for (uint32_t i = 0; i < relationships_read; ++i) {
        mg_unbound_relationship_destroy_ca(relationships[i],
            session->decoder_allocator);
    }
    mg_allocator_free(session->decoder_allocator, relationships);

cleanup_nodes:
    for (uint32_t i = 0; i < nodes_read; ++i) {
        mg_node_destroy_ca(nodes[i], session->decoder_allocator);
    }
    mg_allocator_free(session->decoder_allocator, nodes);
    return status;
}

inline int mg_session_read_date(mg_session* session, mg_date** date) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1), MG_SIGNATURE_DATE));
    mg_date* date_tmp = mg_date_alloc(session->decoder_allocator);
    if (!date_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &date_tmp->days);
    if (status != 0) {
        goto cleanup;
    }

    *date = date_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, date_tmp);
    return status;
}

inline int mg_session_read_time(mg_session* session, mg_time** time) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 2), MG_SIGNATURE_TIME));
    mg_time* time_tmp = mg_time_alloc(session->decoder_allocator);
    if (!time_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &time_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &time_tmp->tz_offset_seconds);
    if (status != 0) {
        goto cleanup;
    }

    *time = time_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, time_tmp);
    return status;
}

inline int mg_session_read_local_time(mg_session* session,
    mg_local_time** local_time) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1), MG_SIGNATURE_LOCAL_TIME));
    mg_local_time* local_time_tmp =
        mg_local_time_alloc(session->decoder_allocator);
    if (!local_time_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &local_time_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    *local_time = local_time_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, local_time_tmp);
    return status;
}

inline int mg_session_read_date_time(mg_session* session, mg_date_time** date_time) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3), MG_SIGNATURE_DATE_TIME));
    mg_date_time* date_time_tmp = mg_date_time_alloc(session->decoder_allocator);
    if (!date_time_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &date_time_tmp->seconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &date_time_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &date_time_tmp->tz_offset_minutes);
    if (status != 0) {
        goto cleanup;
    }

    *date_time = date_time_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, date_time_tmp);
    return status;
}

inline int mg_session_read_date_time_zone_id(
    mg_session* session, mg_date_time_zone_id** date_time_zone_id) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3),
        MG_SIGNATURE_DATE_TIME_ZONE_ID));
    mg_date_time_zone_id* date_time_zone_id_tmp =
        mg_date_time_zone_id_alloc(session->decoder_allocator);
    if (!date_time_zone_id_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &date_time_zone_id_tmp->seconds);
    if (status != 0) {
        goto cleanup;
    }

    status =
        mg_session_read_integer(session, &date_time_zone_id_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &date_time_zone_id_tmp->tz_id);

    *date_time_zone_id = date_time_zone_id_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, date_time_zone_id_tmp);
    return status;
}

inline int mg_session_read_local_date_time(mg_session* session,
    mg_local_date_time** local_date_time) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 2),
        MG_SIGNATURE_LOCAL_DATE_TIME));
    mg_local_date_time* local_date_time_tmp =
        mg_local_date_time_alloc(session->decoder_allocator);
    if (!local_date_time_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &local_date_time_tmp->seconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &local_date_time_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    *local_date_time = local_date_time_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, local_date_time_tmp);
    return status;
}

inline int mg_session_read_duration(mg_session* session, mg_duration** duration) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 4), MG_SIGNATURE_DURATION));
    mg_duration* duration_tmp = mg_duration_alloc(session->decoder_allocator);
    if (!duration_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &duration_tmp->months);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &duration_tmp->days);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &duration_tmp->seconds);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_integer(session, &duration_tmp->nanoseconds);
    if (status != 0) {
        goto cleanup;
    }

    *duration = duration_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, duration_tmp);
    return status;
}

inline int mg_session_read_point_2d(mg_session* session, mg_point_2d** point_2d) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 3), MG_SIGNATURE_POINT_2D));
    mg_point_2d* point_2d_tmp = mg_point_2d_alloc(session->decoder_allocator);
    if (!point_2d_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &point_2d_tmp->srid);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_float(session, &point_2d_tmp->x);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_float(session, &point_2d_tmp->y);
    if (status != 0) {
        goto cleanup;
    }

    *point_2d = point_2d_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, point_2d_tmp);
    return status;
}

inline int mg_session_read_point_3d(mg_session* session, mg_point_3d** point_3d) {
    MG_RETURN_IF_FAILED(mg_session_check_struct_header(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + 4), MG_SIGNATURE_POINT_3D));
    mg_point_3d* point_3d_tmp = mg_point_3d_alloc(session->decoder_allocator);
    if (!point_3d_tmp) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;
    status = mg_session_read_integer(session, &point_3d_tmp->srid);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_float(session, &point_3d_tmp->x);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_float(session, &point_3d_tmp->y);
    if (status != 0) {
        goto cleanup;
    }

    status = mg_session_read_float(session, &point_3d_tmp->z);
    if (status != 0) {
        goto cleanup;
    }

    *point_3d = point_3d_tmp;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, point_3d_tmp);
    return status;
}

inline int mg_session_read_struct_value(mg_session* session, mg_value* value) {
    if (session->in_cursor + 2 > session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    uint8_t* header = (uint8_t*)(session->in_buffer + session->in_cursor);
    uint8_t marker = (uint8_t)header[0];
    uint8_t signature = (uint8_t)header[1];

    if ((marker & 0xF0) != MG_MARKER_TINY_STRUCT) {
        mg_session_set_error(session, "unsupported value");
        return MG_ERROR_DECODING_FAILED;
    }

    switch (signature) {
        case MG_SIGNATURE_NODE:
            value->type = MG_VALUE_TYPE_NODE;
            return mg_session_read_node(session, &value->node_v);
        case MG_SIGNATURE_RELATIONSHIP:
            value->type = MG_VALUE_TYPE_RELATIONSHIP;
            return mg_session_read_relationship(session, &value->relationship_v);
        case MG_SIGNATURE_UNBOUND_RELATIONSHIP:
            value->type = MG_VALUE_TYPE_UNBOUND_RELATIONSHIP;
            return mg_session_read_unbound_relationship(
                session, &value->unbound_relationship_v);
        case MG_SIGNATURE_PATH:
            value->type = MG_VALUE_TYPE_PATH;
            return mg_session_read_path(session, &value->path_v);
        case MG_SIGNATURE_DATE:
            value->type = MG_VALUE_TYPE_DATE;
            return mg_session_read_date(session, &value->date_v);
        case MG_SIGNATURE_TIME:
            value->type = MG_VALUE_TYPE_TIME;
            return mg_session_read_time(session, &value->time_v);
        case MG_SIGNATURE_LOCAL_TIME:
            value->type = MG_VALUE_TYPE_LOCAL_TIME;
            return mg_session_read_local_time(session, &value->local_time_v);
        case MG_SIGNATURE_DATE_TIME:
            value->type = MG_VALUE_TYPE_DATE_TIME;
            return mg_session_read_date_time(session, &value->date_time_v);
        case MG_SIGNATURE_DATE_TIME_ZONE_ID:
            value->type = MG_VALUE_TYPE_DATE_TIME_ZONE_ID;
            return mg_session_read_date_time_zone_id(session,
                &value->date_time_zone_id_v);
        case MG_SIGNATURE_LOCAL_DATE_TIME:
            value->type = MG_VALUE_TYPE_LOCAL_DATE_TIME;
            return mg_session_read_local_date_time(session,
                &value->local_date_time_v);
        case MG_SIGNATURE_DURATION:
            value->type = MG_VALUE_TYPE_DURATION;
            return mg_session_read_duration(session, &value->duration_v);
        case MG_SIGNATURE_POINT_2D:
            value->type = MG_VALUE_TYPE_POINT_2D;
            return mg_session_read_point_2d(session, &value->point_2d_v);
        case MG_SIGNATURE_POINT_3D:
            value->type = MG_VALUE_TYPE_POINT_3D;
            return mg_session_read_point_3d(session, &value->point_3d_v);
    }

    mg_session_set_error(session, "unsupported value");
    return MG_ERROR_DECODING_FAILED;
}

inline int mg_session_read_value(mg_session* session, mg_value** value) {
    if (session->in_cursor >= session->in_end) {
        mg_session_set_error(session, "unexpected end of message");
        return MG_ERROR_DECODING_FAILED;
    }
    uint8_t marker = *(uint8_t*)(session->in_buffer + session->in_cursor);

    mg_value* tvalue =
        (mg_value*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_value));
    if (!tvalue) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    switch (marker) {
        case MG_MARKER_NULL: {
            tvalue->type = MG_VALUE_TYPE_NULL;
            status = mg_session_read_null(session);
            if (status != 0) {
                goto cleanup;
            }
            break;
        }
        case MG_MARKER_BOOL_FALSE:
        case MG_MARKER_BOOL_TRUE:
            tvalue->type = MG_VALUE_TYPE_BOOL;
            status = mg_session_read_bool(session, &tvalue->bool_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_INT_8:
        case MG_MARKER_INT_16:
        case MG_MARKER_INT_32:
        case MG_MARKER_INT_64:
            tvalue->type = MG_VALUE_TYPE_INTEGER;
            status = mg_session_read_integer(session, &tvalue->integer_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_FLOAT:
            tvalue->type = MG_VALUE_TYPE_FLOAT;
            status = mg_session_read_float(session, &tvalue->float_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_STRING_8:
        case MG_MARKER_STRING_16:
        case MG_MARKER_STRING_32:
            tvalue->type = MG_VALUE_TYPE_STRING;
            status = mg_session_read_string(session, &tvalue->string_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_LIST_8:
        case MG_MARKER_LIST_16:
        case MG_MARKER_LIST_32:
            tvalue->type = MG_VALUE_TYPE_LIST;
            status = mg_session_read_list(session, &tvalue->list_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_MAP_8:
        case MG_MARKER_MAP_16:
        case MG_MARKER_MAP_32:
            tvalue->type = MG_VALUE_TYPE_MAP;
            status = mg_session_read_map(session, &tvalue->map_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_MARKER_STRUCT_8:
        case MG_MARKER_STRUCT_16:
            mg_session_set_error(session, "unsupported value");
            status = MG_ERROR_DECODING_FAILED;
            goto cleanup;
        default:
            if ((marker & 0x80) == 0 || (marker & 0xF0) == 0xF0) {
                tvalue->type = MG_VALUE_TYPE_INTEGER;
                status = mg_session_read_integer(session, &tvalue->integer_v);
                if (status != 0) {
                    goto cleanup;
                }
            } else if ((marker & 0xF0) == MG_MARKER_TINY_STRING) {
                tvalue->type = MG_VALUE_TYPE_STRING;
                status = mg_session_read_string(session, &tvalue->string_v);
                if (status != 0) {
                    goto cleanup;
                }
            } else if ((marker & 0xF0) == MG_MARKER_TINY_LIST) {
                tvalue->type = MG_VALUE_TYPE_LIST;
                status = mg_session_read_list(session, &tvalue->list_v);
                if (status != 0) {
                    goto cleanup;
                }
            } else if ((marker & 0xF0) == MG_MARKER_TINY_MAP) {
                tvalue->type = MG_VALUE_TYPE_MAP;
                status = mg_session_read_map(session, &tvalue->map_v);
                if (status != 0) {
                    goto cleanup;
                }
            } else if ((marker & 0xF0) == MG_MARKER_TINY_STRUCT) {
                status = mg_session_read_struct_value(session, tvalue);
                if (status != 0) {
                    goto cleanup;
                }
            } else {
                mg_session_set_error(session, "unsupported value");
                status = MG_ERROR_DECODING_FAILED;
                goto cleanup;
            }
    }

    *value = tvalue;
    return 0;

cleanup:
    mg_allocator_free(session->decoder_allocator, tvalue);
    return status;
}

// Some of these message types are never received by client, but we still have
// decoding function because they are useful for testing.
inline int mg_session_read_success_message(mg_session* session,
    mg_message_success** message) {
    mg_map* metadata;
    MG_RETURN_IF_FAILED(mg_session_read_map(session, &metadata));

    mg_message_success* tmessage = (mg_message_success*)mg_allocator_malloc(
        session->decoder_allocator, sizeof(mg_message_success));
    if (!tmessage) {
        mg_session_set_error(session, "out of memory");
        mg_map_destroy(metadata);
        return MG_ERROR_OOM;
    }
    tmessage->metadata = metadata;
    *message = tmessage;
    return 0;
}

inline int mg_session_read_record_message(mg_session* session,
    mg_message_record** message) {
    mg_list* fields;
    MG_RETURN_IF_FAILED(mg_session_read_list(session, &fields));

    mg_message_record* tmessage = (mg_message_record*)mg_allocator_malloc(session->decoder_allocator,
        sizeof(mg_message_record));
    if (!tmessage) {
        mg_session_set_error(session, "out of memory");
        mg_list_destroy(fields);
        return MG_ERROR_OOM;
    }
    tmessage->fields = fields;
    *message = tmessage;
    return 0;
}

inline int mg_session_read_failure_message(mg_session* session,
    mg_message_failure** message) {
    mg_map* metadata;
    MG_RETURN_IF_FAILED(mg_session_read_map(session, &metadata));

    mg_message_failure* tmessage = (mg_message_failure*)mg_allocator_malloc(
        session->decoder_allocator, sizeof(mg_message_failure));
    if (!tmessage) {
        mg_session_set_error(session, "out of memory");
        mg_map_destroy(metadata);
        return MG_ERROR_OOM;
    }
    tmessage->metadata = metadata;
    *message = tmessage;
    return 0;
}

inline int mg_session_read_init_message(mg_session* session,
    mg_message_init** message) {
    mg_string* client_name;
    MG_RETURN_IF_FAILED(mg_session_read_string(session, &client_name));

    int status = 0;

    mg_map* auth_token;
    mg_message_init* tmessage;
    status = mg_session_read_map(session, &auth_token);
    if (status != 0) {
        mg_string_destroy_ca(client_name, session->decoder_allocator);
        goto cleanup_client_name;
    }

    tmessage =
        (mg_message_init*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message_init));
    if (!tmessage) {
        status = MG_ERROR_OOM;
        goto cleanup;
    }

    tmessage->client_name = client_name;
    tmessage->auth_token = auth_token;
    *message = tmessage;
    return 0;

cleanup:
    mg_map_destroy_ca(auth_token, session->decoder_allocator);

cleanup_client_name:
    mg_string_destroy_ca(client_name, session->decoder_allocator);
    return status;
}

inline int mg_session_read_hello_message(mg_session* session,
    mg_message_hello** message) {
    int status = 0;

    mg_map* extra;
    status = mg_session_read_map(session, &extra);
    if (status != 0) {
        return status;
    }

    mg_message_hello* tmessage =
        (mg_message_hello*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message_hello));
    if (!tmessage) {
        status = MG_ERROR_OOM;
        goto cleanup;
    }

    tmessage->extra = extra;
    *message = tmessage;
    return 0;

cleanup:
    mg_map_destroy_ca(extra, session->decoder_allocator);
    return status;
}

inline int mg_session_read_run_message(mg_session* session, mg_message_run** message) {
    mg_string* statement;
    MG_RETURN_IF_FAILED(mg_session_read_string(session, &statement));

    int status = 0;

    mg_map* parameters;
    mg_map* extra;
    mg_message_run* tmessage;
    status = mg_session_read_map(session, &parameters);
    if (status != 0) {
        mg_string_destroy_ca(statement, session->decoder_allocator);
        goto cleanup_statement;
    }

    extra = NULL;
    if (session->version == 4) {
        status = mg_session_read_map(session, &extra);
        if (status != 0) {
            goto cleanup_parameters;
        }
    }

    tmessage =
        (mg_message_run*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message_run));
    if (!tmessage) {
        status = MG_ERROR_OOM;
        goto cleanup;
    }

    tmessage->statement = statement;
    tmessage->parameters = parameters;
    tmessage->extra = extra;

    *message = tmessage;
    return 0;

cleanup:
    mg_map_destroy_ca(extra, session->decoder_allocator);

cleanup_parameters:
    mg_map_destroy_ca(parameters, session->decoder_allocator);

cleanup_statement:
    mg_string_destroy_ca(statement, session->decoder_allocator);
    return status;
}

inline int mg_session_read_begin_message(mg_session* session,
    mg_message_begin** message) {
    int status = 0;

    mg_map* extra;
    status = mg_session_read_map(session, &extra);
    if (status != 0) {
        return status;
    }

    mg_message_begin* tmessage =
        (mg_message_begin*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message_begin));
    if (!tmessage) {
        status = MG_ERROR_OOM;
        goto cleanup;
    }

    tmessage->extra = extra;
    *message = tmessage;

cleanup:
    mg_map_destroy_ca(extra, session->decoder_allocator);
    return status;
}

inline int mg_session_read_pull_message(mg_session* session,
    mg_message_pull** message) {
    int status = 0;

    mg_map* extra = NULL;
    if (session->version == 4) {
        status = mg_session_read_map(session, &extra);
        if (status != 0) {
            return status;
        }
    }

    mg_message_pull* tmessage =
        (mg_message_pull*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message_pull));
    if (!tmessage) {
        status = MG_ERROR_OOM;
        goto cleanup;
    }

    tmessage->extra = extra;
    *message = tmessage;

cleanup:
    mg_map_destroy_ca(extra, session->decoder_allocator);
    return status;
}

inline int mg_session_read_bolt_message(mg_session* session, mg_message** message) {
    uint8_t marker;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &marker));

    uint8_t signature;
    MG_RETURN_IF_FAILED(mg_session_read_uint8(session, &signature));

    mg_message* tmessage =
        (mg_message*)mg_allocator_malloc(session->decoder_allocator, sizeof(mg_message));
    if (!tmessage) {
        mg_session_set_error(session, "out of memory");
        return MG_ERROR_OOM;
    }

    int status = 0;

    switch (signature) {
        case MG_SIGNATURE_MESSAGE_SUCCESS:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 1)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_SUCCESS;
            status = mg_session_read_success_message(session, &tmessage->success_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_SIGNATURE_MESSAGE_FAILURE:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 1)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_FAILURE;
            status = mg_session_read_failure_message(session, &tmessage->failure_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_SIGNATURE_MESSAGE_RECORD:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 1)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_RECORD;
            status = mg_session_read_record_message(session, &tmessage->record_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_SIGNATURE_MESSAGE_HELLO:
            if (session->version == 1) {
                if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 2)) {
                    goto wrong_marker;
                }
                tmessage->type = MG_MESSAGE_TYPE_INIT;
                status = mg_session_read_init_message(session, &tmessage->init_v);
            } else {
                if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 1)) {
                    goto wrong_marker;
                }
                tmessage->type = MG_MESSAGE_TYPE_HELLO;
                status = mg_session_read_hello_message(session, &tmessage->hello_v);
            }
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_SIGNATURE_MESSAGE_RUN: {
            int field_number = 2 + (session->version == 4);
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + field_number)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_RUN;
            status = mg_session_read_run_message(session, &tmessage->run_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        }
        case MG_SIGNATURE_MESSAGE_ACK_FAILURE:
            if (marker != MG_MARKER_TINY_STRUCT) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_ACK_FAILURE;
            break;
        case MG_SIGNATURE_MESSAGE_BEGIN:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT + 1)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_BEGIN;
            status = mg_session_read_begin_message(session, &tmessage->begin_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        case MG_SIGNATURE_MESSAGE_COMMIT:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_COMMIT;
            break;
        case MG_SIGNATURE_MESSAGE_ROLLBACK:
            if (marker != (uint8_t)(MG_MARKER_TINY_STRUCT)) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_ROLLBACK;
            break;
        case MG_SIGNATURE_MESSAGE_RESET:
            if (marker != MG_MARKER_TINY_STRUCT) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_RESET;
            break;
        case MG_SIGNATURE_MESSAGE_PULL: {
            uint8_t expected_marker = MG_MARKER_TINY_STRUCT + (session->version == 4);
            if (marker != expected_marker) {
                goto wrong_marker;
            }
            tmessage->type = MG_MESSAGE_TYPE_PULL;
            status = mg_session_read_pull_message(session, &tmessage->pull_v);
            if (status != 0) {
                goto cleanup;
            }
            break;
        }
        default:
            mg_session_set_error(session, "unknown message type");
            status = MG_ERROR_PROTOCOL_VIOLATION;
            goto cleanup;
    }

    *message = tmessage;
    return 0;

wrong_marker:
    mg_session_set_error(session, "wrong value marker");
    status = MG_ERROR_PROTOCOL_VIOLATION;
cleanup:
    mg_allocator_free(session->decoder_allocator, tmessage);
    return status;
}

//mgsession-encoder.c-------------------------------------------------------------------------


inline int mg_session_write_uint8(mg_session* session, uint8_t val) {
    return mg_session_write_raw(session, (char*)&val, 1);
}

inline int mg_session_write_uint16(mg_session* session, uint16_t val) {
    val = htobe16(val);
    return mg_session_write_raw(session, (char*)&val, 2);
}

inline int mg_session_write_uint32(mg_session* session, uint32_t val) {
    val = htobe32(val);
    return mg_session_write_raw(session, (char*)&val, 4);
}

inline int mg_session_write_uint64(mg_session* session, uint64_t val) {
    val = htobe64(val);
    return mg_session_write_raw(session, (char*)&val, 8);
}

inline int mg_session_write_null(mg_session* session) {
    return mg_session_write_uint8(session, MG_MARKER_NULL);
}

inline int mg_session_write_bool(mg_session* session, int value) {
    return mg_session_write_uint8(
        session, value ? MG_MARKER_BOOL_TRUE : MG_MARKER_BOOL_FALSE);
}

inline int mg_session_write_integer(mg_session* session, int64_t value) {
    if (value >= MG_TINY_INT_MIN && value <= MG_TINY_INT_MAX) {
        return mg_session_write_uint8(session, (uint8_t)value);
    }
    if (value >= INT8_MIN && value <= INT8_MAX) {
        MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_INT_8));
        return mg_session_write_uint8(session, (uint8_t)value);
    }
    if (value >= INT16_MIN && value <= INT16_MAX) {
        MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_INT_16));
        return mg_session_write_uint16(session, (uint16_t)value);
    }
    if (value >= INT32_MIN && value <= INT32_MAX) {
        MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_INT_32));
        return mg_session_write_uint32(session, (uint32_t)value);
    }
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_INT_64));
    return mg_session_write_uint64(session, (uint64_t)value);
}

/// Markers have to be ordered from smallest to largest.
inline int mg_session_write_container_size(mg_session* session, uint32_t size,
    const uint8_t* markers) {
    if (size <= MG_TINY_SIZE_MAX) {
        return mg_session_write_uint8(session, (uint8_t)(markers[0] + size));
    }
    if (size <= UINT8_MAX) {
        MG_RETURN_IF_FAILED(mg_session_write_uint8(session, markers[1]));
        return mg_session_write_uint8(session, (uint8_t)size);
    }
    if (size <= UINT16_MAX) {
        MG_RETURN_IF_FAILED(mg_session_write_uint8(session, markers[2]));
        return mg_session_write_uint16(session, (uint16_t)size);
    }
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, markers[3]));
    return mg_session_write_uint32(session, size);
}

inline int mg_session_write_float(mg_session* session, double value) {
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_FLOAT));
    uint64_t as_uint64;
    memcpy(&as_uint64, &value, sizeof(value));
    return mg_session_write_uint64(session, as_uint64);
}

inline int mg_session_write_string2(mg_session* session, uint32_t len,
    const char* data) {
    MG_RETURN_IF_FAILED(
        mg_session_write_container_size(session, len, MG_MARKERS_STRING));
    return mg_session_write_raw(session, data, len) != 0;
}

inline int mg_session_write_string(mg_session* session, const char* str) {
    size_t len = strlen(str);
    if (len > UINT32_MAX) {
        mg_session_set_error(session, "string too long");
        return MG_ERROR_SIZE_EXCEEDED;
    }
    return mg_session_write_string2(session, (uint32_t)len, str);
}

inline int mg_session_write_list(mg_session* session, const mg_list* list) {
    MG_RETURN_IF_FAILED(
        mg_session_write_container_size(session, list->size, MG_MARKERS_LIST));
    for (uint32_t i = 0; i < list->size; ++i) {
        MG_RETURN_IF_FAILED(mg_session_write_value(session, list->elements[i]));
    }
    return 0;
}

inline int mg_session_write_map(mg_session* session, const mg_map* map) {
    MG_RETURN_IF_FAILED(
        mg_session_write_container_size(session, map->size, MG_MARKERS_MAP));
    for (uint32_t i = 0; i < map->size; ++i) {
        MG_RETURN_IF_FAILED(mg_session_write_string2(session, map->keys[i]->size,
            map->keys[i]->data));
        MG_RETURN_IF_FAILED(mg_session_write_value(session, map->values[i]));
    }
    return 0;
}

inline int mg_session_write_date(mg_session* session, const mg_date* date) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT1)));
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_SIGNATURE_DATE));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, date->days));
    return 0;
}

inline int mg_session_write_local_time(mg_session* session, const mg_local_time* lt) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT1)));
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_SIGNATURE_LOCAL_TIME));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, lt->nanoseconds));
    return 0;
}

inline int mg_session_write_local_date_time(mg_session* session,
    const mg_local_date_time* ldt) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT2)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_LOCAL_DATE_TIME));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, ldt->seconds));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, ldt->nanoseconds));
    return 0;
}

inline int mg_session_write_duration(mg_session* session, const mg_duration* dur) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT4)));
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_SIGNATURE_DURATION));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, dur->months));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, dur->days));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, dur->seconds));
    MG_RETURN_IF_FAILED(mg_session_write_integer(session, dur->nanoseconds));
    return 0;
}

inline int mg_session_write_value(mg_session* session, const mg_value* value) {
    switch (value->type) {
        case MG_VALUE_TYPE_NULL:
            return mg_session_write_null(session);
        case MG_VALUE_TYPE_BOOL:
            return mg_session_write_bool(session, value->bool_v);
        case MG_VALUE_TYPE_INTEGER:
            return mg_session_write_integer(session, value->integer_v);
        case MG_VALUE_TYPE_FLOAT:
            return mg_session_write_float(session, value->float_v);
        case MG_VALUE_TYPE_STRING:
            return mg_session_write_string2(session, value->string_v->size,
                value->string_v->data);
        case MG_VALUE_TYPE_LIST:
            return mg_session_write_list(session, value->list_v);
        case MG_VALUE_TYPE_MAP:
            return mg_session_write_map(session, value->map_v);
        case MG_VALUE_TYPE_NODE:
            mg_session_set_error(session, "tried to send value of type 'node'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_RELATIONSHIP:
            mg_session_set_error(session,
                "tried to send value of type 'relationship'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_UNBOUND_RELATIONSHIP:
            mg_session_set_error(
                session, "tried to send value of type 'unbound_relationship'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_PATH:
            mg_session_set_error(session, "tried to send value of type 'path'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_DATE:
            return mg_session_write_date(session, value->date_v);
        case MG_VALUE_TYPE_TIME:
            mg_session_set_error(session, "tried to send value of type 'time'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_LOCAL_TIME:
            return mg_session_write_local_time(session, value->local_time_v);
        case MG_VALUE_TYPE_DATE_TIME:
            mg_session_set_error(session, "tried to send value of type 'date_time'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_DATE_TIME_ZONE_ID:
            mg_session_set_error(session,
                "tried to send value of type 'date_time_zone_id'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_LOCAL_DATE_TIME:
            return mg_session_write_local_date_time(session,
                value->local_date_time_v);
        case MG_VALUE_TYPE_DURATION:
            return mg_session_write_duration(session, value->duration_v);
        case MG_VALUE_TYPE_POINT_2D:
            mg_session_set_error(session, "tried to send value of type 'point_2d'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_POINT_3D:
            mg_session_set_error(session, "tried to send value of type 'point_3d'");
            return MG_ERROR_INVALID_VALUE;
        case MG_VALUE_TYPE_UNKNOWN:
            mg_session_set_error(session, "tried to send value of unknown type");
            return MG_ERROR_INVALID_VALUE;
    }
    // Should never get here.
    abort();
}

// Some of these message types are never sent by client, but we still have
// encoding function because they are useful for testing.
inline int mg_session_send_init_message(mg_session* session, const char* client_name,
    const mg_map* auth_token) {
    size_t client_name_len = strlen(client_name);
    if (client_name_len > UINT32_MAX) {
        return MG_ERROR_SIZE_EXCEEDED;
    }
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT + 2)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_HELLO));
    MG_RETURN_IF_FAILED(mg_session_write_string2(
        session, (uint32_t)client_name_len, client_name));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, auth_token));
    return mg_session_flush_message(session);
}

inline int mg_session_send_hello_message(mg_session* session, const mg_map* extra) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_HELLO));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, extra));
    return mg_session_flush_message(session);
}

inline int mg_session_send_run_message(mg_session* session, const char* statement,
    const mg_map* parameters, const mg_map* extra) {
    int field_number = 2 + (session->version == 4);
    MG_RETURN_IF_FAILED(mg_session_write_uint8(
        session, (uint8_t)(MG_MARKER_TINY_STRUCT + field_number)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_RUN));
    MG_RETURN_IF_FAILED(mg_session_write_string(session, statement));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, parameters));

    if (session->version == 4) {
        MG_RETURN_IF_FAILED(mg_session_write_map(session, extra));
    }
    return mg_session_flush_message(session);
}

inline int mg_session_send_pull_message(mg_session* session, const mg_map* extra) {
    uint8_t marker = MG_MARKER_TINY_STRUCT + (session->version == 4);
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, marker));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_PULL));

    if (session->version == 4) {
        MG_RETURN_IF_FAILED(mg_session_write_map(session, extra));
    }

    return mg_session_flush_message(session);
}

inline int mg_session_send_ack_failure_message(mg_session* session) {
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_TINY_STRUCT));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_ACK_FAILURE));
    return mg_session_flush_message(session);
}

inline int mg_session_send_reset_message(mg_session* session) {
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, MG_MARKER_TINY_STRUCT));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_RESET));
    return mg_session_flush_message(session);
}

inline int mg_session_send_failure_message(mg_session* session,
    const mg_map* metadata) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_FAILURE));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, metadata));
    return mg_session_flush_message(session);
}

inline int mg_session_send_success_message(mg_session* session,
    const mg_map* metadata) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_SUCCESS));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, metadata));
    return mg_session_flush_message(session);
}

inline int mg_session_send_record_message(mg_session* session, const mg_list* fields) {
    MG_RETURN_IF_FAILED(mg_session_write_uint8(session, 0xB1));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_RECORD));
    MG_RETURN_IF_FAILED(mg_session_write_list(session, fields));
    return mg_session_flush_message(session);
}

inline int mg_session_send_begin_message(mg_session* session, const mg_map* extra) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT + 1)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_BEGIN));
    MG_RETURN_IF_FAILED(mg_session_write_map(session, extra));
    return mg_session_flush_message(session);
}

inline int mg_session_send_commit_messsage(mg_session* session) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_COMMIT));
    return mg_session_flush_message(session);
}

inline int mg_session_send_rollback_messsage(mg_session* session) {
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, (uint8_t)(MG_MARKER_TINY_STRUCT)));
    MG_RETURN_IF_FAILED(
        mg_session_write_uint8(session, MG_SIGNATURE_MESSAGE_ROLLBACK));
    return mg_session_flush_message(session);
}

//mgsocket.c-------------------------------------------------------------------------

// Please refer to https://docs.microsoft.com/en-us/windows/win32/api/winsock2/
// for more details about Windows system calls.

inline int mg_socket_init() {
    WSADATA data;
    int status = WSAStartup(MAKEWORD(2, 2), &data);
    if (status != 0) {
        fprintf(stderr, "WSAStartup failed: %s\n", mg_socket_error());
        abort();
    }
    return MG_SUCCESS;
}

inline int mg_socket_create(int af, int type, int protocol) {
    SOCKET sock = socket(af, type, protocol);
    // Useful info here https://stackoverflow.com/questions/10817252.
    if (sock == INVALID_SOCKET) {
        return MG_ERROR_SOCKET;
    }
    if (sock > INT_MAX) {
        fprintf(stderr,
            "Implementation is wrong. Unsigned result of socket system call "
            "can not be stored to signed data type. Please contact the"
            "maintainer.\n");
        abort();
    }
    return (int)sock;
}

inline int mg_socket_create_handle_error(int sock, mg_session* session) {
    if (sock == MG_ERROR_SOCKET) {
        mg_session_set_error(session, "couldn't open socket: %s",
            mg_socket_error());
        return MG_ERROR_NETWORK_FAILURE;
    }
    return MG_SUCCESS;
}

inline int mg_socket_connect(int sock, const struct sockaddr* addr,
    socklen_t addrlen) {
    int status = connect(sock, addr, addrlen);
    if (status != 0) {
        return MG_ERROR_SOCKET;
    }
    return MG_SUCCESS;
}

inline int mg_socket_connect_handle_error(int* sock, int status, mg_session* session) {
    if (status != MG_SUCCESS) {
        mg_session_set_error(session, "couldn't connect to host: %s",
            mg_socket_error());
        if (mg_socket_close(*sock) != 0) {
            abort();
        }
        *sock = MG_ERROR_SOCKET;
        return MG_ERROR_NETWORK_FAILURE;
    }
    return MG_SUCCESS;
}

inline int mg_socket_options(int sock, mg_session* session) {
    (void)sock;
    (void)session;
    return MG_SUCCESS;
}

inline ssize_t mg_socket_send(int sock, const void* buf, int len) {
    int sent = send(sock, (const char*)buf, len, 0);
    if (sent == SOCKET_ERROR) {
        return -1;
    }
    return sent;
}

inline ssize_t mg_socket_receive(int sock, void* buf, int len) {
    int received = recv(sock, (char*)buf, len, 0);
    if (received == SOCKET_ERROR) {
        return -1;
    }
    return received;
}

inline int mg_socket_poll(struct pollfd* fds, unsigned int nfds, int timeout) {
    return WSAPoll(fds, nfds, timeout);
}

// Implementation here
// https://nlnetlabs.nl/svn/unbound/tags/release-1.0.1/compat/socketpair.c
// does not work.
inline int mg_socket_pair(int d, int type, int protocol, int* sv) {
    (void)d;
    (void)type;
    (void)protocol;
    (void)sv;
    return MG_ERROR_UNIMPLEMENTED;
}

inline int mg_socket_close(int sock) {
    int shutdown_status = shutdown(sock, SD_BOTH);
    if (shutdown_status != 0) {
        fprintf(stderr, "Fail to shutdown socket: %s\n", mg_socket_error());
    }
    int closesocket_status = closesocket(sock);
    if (closesocket_status != 0) {
        fprintf(stderr, "Fail to close socket: %s\n", mg_socket_error());
    }
    return MG_SUCCESS;
}

inline char* mg_socket_error() {
    // FormatMessage could be used but a caller would have
    // to take care of the allocated memory (LocalFree call).
    switch (WSAGetLastError()) {
        case WSA_INVALID_HANDLE:
            return (char*)"Specified event object handle is invalid.";
        case WSA_NOT_ENOUGH_MEMORY:
            return (char*)"Insufficient memory available.";
        case WSA_INVALID_PARAMETER:
            return (char*)"One or more parameters are invalid.";
        case WSA_OPERATION_ABORTED:
            return (char*)"Overlapped operation aborted.";
        case WSA_IO_INCOMPLETE:
            return (char*)"Overlapped I/O event object not in signaled state.";
        case WSA_IO_PENDING:
            return (char*)"Overlapped operations will complete later.";
        case WSAEINTR:
            return (char*)"Interrupted function call.";
        case WSAEBADF:
            return (char*)"File handle is not valid.";
        case WSAEACCES:
            return (char*)"Permission denied.";
        case WSAEFAULT:
            return (char*)"Bad address.";
        case WSAEINVAL:
            return (char*)"Invalid argument.";
        case WSAEMFILE:
            return (char*)"Too many open files.";
        case WSAEWOULDBLOCK:
            return (char*)"Resource temporarily unavailable.";
        case WSAEINPROGRESS:
            return (char*)"Operation now in progress.";
        case WSAEALREADY:
            return (char*)"Operation already in progress.";
        case WSAENOTSOCK:
            return (char*)"Socket operation on nonsocket.";
        case WSAEDESTADDRREQ:
            return (char*)"Destination address required.";
        case WSAEMSGSIZE:
            return (char*)"Message too long.";
        case WSAEPROTOTYPE:
            return (char*)"Protocol wrong type for socket.";
        case WSAENOPROTOOPT:
            return (char*)"Bad protocol option.";
        case WSAEPROTONOSUPPORT:
            return (char*)"Protocol not supported.";
        case WSAESOCKTNOSUPPORT:
            return (char*)"Socket type not supported.";
        case WSAEOPNOTSUPP:
            return (char*)"Operation not supported.";
        case WSAEPFNOSUPPORT:
            return (char*)"Protocol family not supported.";
        case WSAEAFNOSUPPORT:
            return (char*)"Address family not supported by protocol family.";
        case WSAEADDRINUSE:
            return (char*)"Address already in use.";
        case WSAEADDRNOTAVAIL:
            return (char*)"Cannot assign requested address.";
        case WSAENETDOWN:
            return (char*)"Network is down.";
        case WSAENETUNREACH:
            return (char*)"Network is unreachable.";
        case WSAENETRESET:
            return (char*)"Network dropped connection on reset.";
        case WSAECONNABORTED:
            return (char*)"Software caused connection abort.";
        case WSAECONNRESET:
            return (char*)"Connection reset by peer.";
        case WSAENOBUFS:
            return (char*)"No buffer space available.";
        case WSAEISCONN:
            return (char*)"Socket is already connected.";
        case WSAENOTCONN:
            return (char*)"Socket is not connected.";
        case WSAESHUTDOWN:
            return (char*)"Cannot send after socket shutdown.";
        case WSAETOOMANYREFS:
            return (char*)"Too many references to some kernel object.";
        case WSAETIMEDOUT:
            return (char*)"Connection timed out.";
        case WSAECONNREFUSED:
            return (char*)"Connection refused.";
        case WSAELOOP:
            return (char*)"Cannot translate name.";
        case WSAENAMETOOLONG:
            return (char*)"Name too long.";
        case WSAEHOSTDOWN:
            return (char*)"Host is down.";
        case WSAEHOSTUNREACH:
            return (char*)"No route to host.";
        case WSAENOTEMPTY:
            return (char*)"Directory not empty.";
        case WSAEPROCLIM:
            return (char*)"Too many processes.";
        case WSAEUSERS:
            return (char*)"User quota exceeded.";
        case WSAEDQUOT:
            return (char*)"Disk quota exceeded.";
        case WSAESTALE:
            return (char*)"Stale file handle reference.";
        case WSAEREMOTE:
            return (char*)"Item is remote.";
        case WSASYSNOTREADY:
            return (char*)"Network subsystem is unavailable.";
        case WSAVERNOTSUPPORTED:
            return (char*)"Winsock.dll version out of range.";
        case WSANOTINITIALISED:
            return (char*)"Successful WSAStartup not yet performed.";
        case WSAEDISCON:
            return (char*)"Graceful shutdown in progress.";
        case WSAENOMORE:
            return (char*)"No more results.";
        case WSAECANCELLED:
            return (char*)"Call has been canceled.";
        case WSAEINVALIDPROCTABLE:
            return (char*)"Procedure call table is invalid.";
        case WSAEINVALIDPROVIDER:
            return (char*)"Service provider is invalid.";
        case WSAEPROVIDERFAILEDINIT:
            return (char*)"Service provider failed to initialize.";
        case WSASYSCALLFAILURE:
            return (char*)"System call failure.";
        case WSASERVICE_NOT_FOUND:
            return (char*)"Service not found.";
        case WSATYPE_NOT_FOUND:
            return (char*)"Class type not found.";
        case WSA_E_NO_MORE:
            return (char*)"No more results.";
        case WSA_E_CANCELLED:
            return (char*)"Call was canceled.";
        case WSAEREFUSED:
            return (char*)"Database query was refused.";
        case WSAHOST_NOT_FOUND:
            return (char*)"Host not found.";
        case WSATRY_AGAIN:
            return (char*)"Nonauthoritative host not found.";
        case WSANO_RECOVERY:
            return (char*)"This is a nonrecoverable error.";
        case WSANO_DATA:
            return (char*)"Valid name, no data record of requested type.";
        case WSA_QOS_RECEIVERS:
            return (char*)"At least one QoS reserve has arrived.";
        case WSA_QOS_SENDERS:
            return (char*)"At least one QoS send path has arrived.";
        case WSA_QOS_NO_SENDERS:
            return (char*)"There are no QoS senders.";
        case WSA_QOS_NO_RECEIVERS:
            return (char*)"There are no QoS receivers.";
        case WSA_QOS_REQUEST_CONFIRMED:
            return (char*)"The QoS reserve request has been confirmed.";
        case WSA_QOS_ADMISSION_FAILURE:
            return (char*)"QoS admission error.";
        case WSA_QOS_POLICY_FAILURE:
            return (char*)"QoS policy failure.";
        case WSA_QOS_BAD_STYLE:
            return (char*)"QoS bad style.";
        case WSA_QOS_BAD_OBJECT:
            return (char*)"QoS bad object.";
        case WSA_QOS_TRAFFIC_CTRL_ERROR:
            return (char*)"QoS traffic control error.";
        case WSA_QOS_GENERIC_ERROR:
            return (char*)"QoS generic error.";
        case WSA_QOS_ESERVICETYPE:
            return (char*)"QoS service type error.";
        case WSA_QOS_EFLOWSPEC:
            return (char*)"QoS flowspec error.";
        case WSA_QOS_EPROVSPECBUF:
            return (char*)"Invalid QoS provider buffer.";
        case WSA_QOS_EFILTERSTYLE:
            return (char*)"Invalid QoS filter style.";
        case WSA_QOS_EFILTERTYPE:
            return (char*)"Invalid QoS filter type.";
        case WSA_QOS_EFILTERCOUNT:
            return (char*)"Incorrect QoS filter count.";
        case WSA_QOS_EOBJLENGTH:
            return (char*)"Invalid QoS object length.";
        case WSA_QOS_EFLOWCOUNT:
            return (char*)"Incorrect QoS flow count.";
        case WSA_QOS_EUNKOWNPSOBJ:
            return (char*)"Unrecognized QoS object.";
        case WSA_QOS_EPOLICYOBJ:
            return (char*)"Invalid QoS policy object.";
        case WSA_QOS_EFLOWDESC:
            return (char*)"Invalid QoS flow descriptor.";
        case WSA_QOS_EPSFLOWSPEC:
            return (char*)"Invalid QoS provider-specific flowspec.";
        case WSA_QOS_EPSFILTERSPEC:
            return (char*)"Invalid QoS provider-specific filterspec.";
        case WSA_QOS_ESDMODEOBJ:
            return (char*)"Invalid QoS shape discard mode object.";
        case WSA_QOS_ESHAPERATEOBJ:
            return (char*)"Invalid QoS shaping rate object.";
        case WSA_QOS_RESERVED_PETYPE:
            return (char*)"Reserved policy QoS element type.";
        default:
            return (char*)"Unknown WSA error.";
    }
    return (char*)"Unknown WSA error.";
}

inline void mg_socket_finalize() {
    if (WSACleanup() != 0) {
        fprintf(stderr, "WSACleanup failed: %s\n", mg_socket_error());
    }
}

//mgtransport.c-------------------------------------------------------------------------

inline int mg_init_ssl = 1;

inline int mg_transport_send(mg_transport* transport, const char* buf, size_t len) {
    return transport->send(transport, buf, len);
}

inline int mg_transport_recv(mg_transport* transport, char* buf, size_t len) {
    return transport->recv(transport, buf, len);
}

inline void mg_transport_destroy(mg_transport* transport) {
    transport->destroy(transport);
}

inline void mg_transport_suspend_until_ready_to_read(struct mg_transport* transport) {
    if (transport->suspend_until_ready_to_read) {
        transport->suspend_until_ready_to_read(transport);
    }
}

inline void mg_transport_suspend_until_ready_to_write(struct mg_transport* transport) {
    if (transport->suspend_until_ready_to_write) {
        transport->suspend_until_ready_to_write(transport);
    }
}

inline int mg_raw_transport_init(int sockfd, mg_raw_transport** transport,
    mg_allocator* allocator) {
    mg_raw_transport* ttransport =
        (mg_raw_transport*)mg_allocator_malloc(allocator, sizeof(mg_raw_transport));
    if (!ttransport) {
        return MG_ERROR_OOM;
    }
    ttransport->sockfd = sockfd;
    ttransport->send = mg_raw_transport_send;
    ttransport->recv = mg_raw_transport_recv;
    ttransport->destroy = mg_raw_transport_destroy;
    ttransport->suspend_until_ready_to_read =
        mg_raw_transport_suspend_until_ready_to_read;
    ttransport->suspend_until_ready_to_write =
        mg_raw_transport_suspend_until_ready_to_write;
    ttransport->allocator = allocator;
    *transport = ttransport;
    return 0;
}

inline int mg_raw_transport_send(struct mg_transport* transport, const char* buf,
    size_t len) {
    int sockfd = ((mg_raw_transport*)transport)->sockfd;
    size_t total_sent = 0;
    while (total_sent < len) {
        // TODO(mtomic): maybe enable using MSG_MORE here
        ssize_t sent_now =
            mg_socket_send(sockfd, buf + total_sent, len - total_sent);
        if (sent_now == -1) {
            perror("mg_raw_transport_send");
            return -1;
        }
        total_sent += (size_t)sent_now;
    }
    return 0;
}

inline int mg_raw_transport_recv(struct mg_transport* transport, char* buf,
    size_t len) {
    int sockfd = ((mg_raw_transport*)transport)->sockfd;
    size_t total_received = 0;
    while (total_received < len) {
        ssize_t received_now =
            mg_socket_receive(sockfd, buf + total_received, len - total_received);
        if (received_now == 0) {
            // Server closed the connection.
            fprintf(stderr, "mg_raw_transport_recv: connection closed by server\n");
            return -1;
        }
        if (received_now == -1) {
            perror("mg_raw_transport_recv");
            return -1;
        }
        total_received += (size_t)received_now;
    }
    return 0;
}

inline void mg_raw_transport_destroy(struct mg_transport* transport) {
    mg_raw_transport* self = (mg_raw_transport*)transport;
    if (mg_socket_close(self->sockfd) != 0) {
        abort();
    }
    mg_allocator_free(self->allocator, transport);
}

inline void mg_raw_transport_suspend_until_ready_to_read(
    struct mg_transport* transport) {
#ifdef __EMSCRIPTEN__
    const int sock = ((mg_raw_transport*)transport)->sockfd;
    mg_wasm_suspend_until_ready_to_read(sock);
#else
    (void)transport;
#endif
}

inline void mg_raw_transport_suspend_until_ready_to_write(
    struct mg_transport* transport) {
#ifdef __EMSCRIPTEN__
    const int sock = ((mg_raw_transport*)transport)->sockfd;
    mg_wasm_suspend_until_ready_to_write(sock);
#else
    (void)transport;
#endif
}

#ifndef __EMSCRIPTEN__
inline static int print_ssl_error(const char* str, size_t len, void* u) {
    (void)len;
    fprintf(stderr, "%s: %s", (char*)u, str);
    return 0;
}

inline static char* hex_encode(unsigned char* data, unsigned int len,
    mg_allocator* allocator) {
    char* encoded = (char*)mg_allocator_malloc(allocator, 2 * len + 1);
    for (unsigned int i = 0; i < len; ++i) {
        sprintf(encoded + 2 * i, "%02x", data[i]);
    }
    return encoded;
}

#ifdef MGCLIENT_ENABLE_SSL
inline static void mg_openssl_init() {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static int mg_ssl_initialized = 0;
    pthread_mutex_lock(&mutex);
    if (mg_init_ssl && !mg_ssl_initialized) {
        printf("initializing openssl\n");
        SSL_library_init();
        SSL_load_error_strings();
        ERR_load_crypto_strings();
        mg_ssl_initialized = 1;
    }
    pthread_mutex_unlock(&mutex);
#endif
}

inline int mg_secure_transport_init(int sockfd, const char* cert_file,
    const char* key_file,
    mg_secure_transport** transport,
    mg_allocator* allocator) {
    mg_openssl_init();

    SSL_CTX* ctx = NULL;
    SSL* ssl = NULL;
    BIO* bio = NULL;

    int status = 0;

    ERR_clear_error();

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ctx = SSL_CTX_new(SSLv23_client_method());
#else
    ctx = SSL_CTX_new(TLS_client_method());
#endif
    int ret;
    X509* peer_cert;
    EVP_PKEY* peer_pubkey;
    const char* peer_pubkey_type;
    mg_secure_transport* ttransport;
    int nid;
    if (!ctx) {
        status = MG_ERROR_SSL_ERROR;
        goto failure;
    }

    if (cert_file && key_file) {
        if (SSL_CTX_use_certificate_chain_file(ctx, cert_file) != 1 ||
            SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) != 1) {
            status = MG_ERROR_SSL_ERROR;
            goto failure;
        }
    }

    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv3);
    ssl = SSL_new(ctx);

    if (!ssl) {
        status = MG_ERROR_SSL_ERROR;
        goto failure;
    }

    // SSL_CTX object is reference counted, we're destroying this local reference,
    // but reference from SSL object stays.
    SSL_CTX_free(ctx);
    ctx = NULL;

    bio = BIO_new_socket(sockfd, BIO_NOCLOSE);
    if (!bio) {
        status = MG_ERROR_SSL_ERROR;
        goto failure;
    }
    SSL_set_bio(ssl, bio, bio);

    ret = SSL_connect(ssl);
    if (ret < 0) {
        status = MG_ERROR_SSL_ERROR;
        goto failure;
    }

    // Get server's public key type and fingerprint.
    peer_cert = SSL_get_peer_certificate(ssl);
    assert(peer_cert);
    peer_pubkey = X509_get_pubkey(peer_cert);
    nid = EVP_PKEY_base_id(peer_pubkey);
    EVP_PKEY_free(peer_pubkey);
    peer_pubkey_type =
        (nid == NID_undef) ? "UNKNOWN" : OBJ_nid2ln(nid);
    unsigned char peer_pubkey_fp[EVP_MAX_MD_SIZE];
    unsigned int peer_pubkey_fp_len;
    if (X509_pubkey_digest(peer_cert, EVP_sha512(), peer_pubkey_fp,
        &peer_pubkey_fp_len) != 1) {
        status = MG_ERROR_SSL_ERROR;
        X509_free(peer_cert);
        goto failure;
    }
    X509_free(peer_cert);

    ttransport =
        (mg_secure_transport*)mg_allocator_malloc(allocator, sizeof(mg_secure_transport));
    if (!ttransport) {
        status = MG_ERROR_OOM;
        goto failure;
    }

    // Take ownership of the socket now that everything went well.
    BIO_set_close(bio, BIO_CLOSE);

    ttransport->ssl = ssl;
    ttransport->bio = bio;
    ttransport->peer_pubkey_type = peer_pubkey_type;
    ttransport->peer_pubkey_fp =
        hex_encode(peer_pubkey_fp, peer_pubkey_fp_len, allocator);
    ttransport->send = mg_secure_transport_send;
    ttransport->recv = mg_secure_transport_recv;
    ttransport->suspend_until_ready_to_read = NULL;
    ttransport->suspend_until_ready_to_write = NULL;
    ttransport->destroy = mg_secure_transport_destroy;
    ttransport->allocator = allocator;
    *transport = ttransport;

    return 0;

failure:
    if (status == MG_ERROR_SSL_ERROR) {
        ERR_print_errors_cb(print_ssl_error, (void*)"mg_secure_transport_init");
    }
    SSL_CTX_free(ctx);
    if (ssl) {
        // If SSL object was successfuly created, it owns the BIO so we don't need
        // to destroy it.
        SSL_free(ssl);
    } else {
        BIO_free(bio);
    }

    return status;
}

inline int mg_secure_transport_send(mg_transport* transport, const char* buf,
    size_t len) {
    SSL* ssl = ((mg_secure_transport*)transport)->ssl;
    BIO* bio = ((mg_secure_transport*)transport)->bio;
    size_t total_sent = 0;
    while (total_sent < len) {
        ERR_clear_error();
        int sent_now = SSL_write(ssl, buf + total_sent, (int)(len - total_sent));
        if (sent_now <= 0) {
            int err = SSL_get_error(ssl, sent_now);
            if (err == SSL_ERROR_WANT_READ) {
                struct pollfd p;
                if (BIO_get_fd(bio, &p.fd) < 0) {
                    abort();
                }
                p.events = POLLIN;
                if (mg_socket_poll(&p, 1, -1) < 0) {
                    return -1;
                }
                continue;
            } else {
                ERR_print_errors_cb(print_ssl_error, (void*)"mg_secure_transport_send");
                return -1;
            }
        }
        assert((size_t)sent_now == len);
        total_sent += (size_t)sent_now;
    }
    return 0;
}

inline int mg_secure_transport_recv(mg_transport* transport, char* buf, size_t len) {
    SSL* ssl = ((mg_secure_transport*)transport)->ssl;
    BIO* bio = ((mg_secure_transport*)transport)->bio;
    size_t total_received = 0;
    while (total_received < len) {
        ERR_clear_error();
        int received_now =
            SSL_read(ssl, buf + total_received, (int)(len - total_received));
        if (received_now <= 0) {
            int err = SSL_get_error(ssl, received_now);
            if (err == SSL_ERROR_WANT_READ) {
                struct pollfd p;
                if (BIO_get_fd(bio, &p.fd) < 0) {
                    abort();
                }
                p.events = POLLIN;
                if (mg_socket_poll(&p, 1, -1) < 0) {
                    return -1;
                }
                continue;
            } else {
                ERR_print_errors_cb(print_ssl_error, (void*)"mg_secure_transport_recv");
                return -1;
            }
        }
        total_received += (size_t)received_now;
    }
    return 0;
}

inline void mg_secure_transport_destroy(mg_transport* transport) {
    mg_secure_transport* self = (mg_secure_transport*)transport;
    SSL_free(self->ssl);
    self->bio = NULL;
    self->ssl = NULL;
    mg_allocator_free(self->allocator, self->peer_pubkey_fp);
    mg_allocator_free(self->allocator, self);
}
#endif

#endif


//mgclient.c-------------------------------------------------------------------------

inline const char* mg_client_version() { return MGCLIENT_VERSION; }

inline int mg_init_session_static_vars() {
    mg_value* n_val = mg_value_make_integer(-1);
    if (!n_val) {
        goto fatal_failure;
    }
    mg_default_pull_extra_map = mg_map_make_empty(1);
    if (!mg_default_pull_extra_map) {
        goto fatal_failure;
    }
    if (mg_map_insert_unsafe(mg_default_pull_extra_map, "n", n_val) != 0) {
        goto fatal_failure;
    }
    return MG_SUCCESS;

fatal_failure:
    if (n_val) {
        mg_value_destroy(n_val);
    }
    if (mg_default_pull_extra_map) {
        mg_map_destroy(mg_default_pull_extra_map);
    }
    return MG_ERROR_CLIENT_ERROR;
}

inline int mg_init() {
    int init_status = mg_init_session_static_vars();
    if (init_status != 0) return init_status;
    return mg_socket_init();
}

inline void mg_finalize() { mg_socket_finalize(); }

typedef struct mg_session_params {
    const char* address;
    const char* host;
    uint16_t port;
    const char* username;
    const char* password;
    const char* user_agent;
    enum mg_sslmode sslmode;
    const char* sslcert;
    const char* sslkey;
    int (*trust_callback)(const char*, const char*, const char*, const char*,
        void*);
    void* trust_data;
} mg_session_params;

inline mg_session_params* mg_session_params_make() {
    mg_session_params* params =
        (mg_session_params*)mg_allocator_malloc(&mg_system_allocator, sizeof(mg_session_params));
    if (!params) {
        return NULL;
    }
    params->address = NULL;
    params->host = NULL;
    params->port = 0;
    params->username = NULL;
    params->password = NULL;
    params->user_agent = MG_USER_AGENT;
    params->sslmode = MG_SSLMODE_DISABLE;
    params->sslcert = NULL;
    params->sslkey = NULL;
    params->trust_callback = NULL;
    params->trust_data = NULL;
    return params;
}

inline void mg_session_params_destroy(mg_session_params* params) {
    if (!params) {
        return;
    }
    mg_allocator_free(&mg_system_allocator, params);
}

inline void mg_session_params_set_address(mg_session_params* params,
    const char* address) {
    params->address = address;
}

inline void mg_session_params_set_host(mg_session_params* params, const char* host) {
    params->host = host;
}

inline void mg_session_params_set_port(mg_session_params* params, uint16_t port) {
    params->port = port;
}

inline void mg_session_params_set_username(mg_session_params* params,
    const char* username) {
    params->username = username;
}

inline void mg_session_params_set_password(mg_session_params* params,
    const char* password) {
    params->password = password;
}

inline void mg_session_params_set_user_agent(mg_session_params* params,
    const char* user_agent) {
    params->user_agent = user_agent;
}

inline void mg_session_params_set_sslmode(mg_session_params* params,
    enum mg_sslmode sslmode) {
    params->sslmode = sslmode;
}

inline void mg_session_params_set_sslcert(mg_session_params* params,
    const char* sslcert) {
    params->sslcert = sslcert;
}

inline void mg_session_params_set_sslkey(mg_session_params* params,
    const char* sslkey) {
    params->sslkey = sslkey;
}

inline void mg_session_params_set_trust_callback(
    mg_session_params* params,
    int (*trust_callback)(const char*, const char*, const char*,
        const char*, void*)) {
    params->trust_callback = trust_callback;
}

inline void mg_session_params_set_trust_data(mg_session_params* params,
    void* trust_data) {
    params->trust_data = trust_data;
}

inline const char* mg_session_params_get_address(const mg_session_params* params) {
    return params->address;
}

inline const char* mg_session_params_get_host(const mg_session_params* params) {
    return params->host;
}

inline uint16_t mg_session_params_get_port(const mg_session_params* params) {
    return params->port;
}

inline const char* mg_session_params_get_username(const mg_session_params* params) {
    return params->username;
}

inline const char* mg_session_params_get_password(const mg_session_params* params) {
    return params->password;
}

inline const char* mg_session_params_get_user_agent(const mg_session_params* params) {
    return params->user_agent;
}

inline enum mg_sslmode mg_session_params_get_sslmode(const mg_session_params* params) {
    return params->sslmode;
}

inline const char* mg_session_params_get_sslcert(const mg_session_params* params) {
    return params->sslcert;
}

inline const char* mg_session_params_get_sslkey(const mg_session_params* params) {
    return params->sslkey;
}

inline int (*mg_session_params_get_trust_callback(const mg_session_params* params))(
    const char*, const char*, const char*, const char*, void*) {
    return params->trust_callback;
}

inline void* mg_session_params_get_trust_data(const mg_session_params* params) {
    return params->trust_data;
}

inline int validate_session_params(const mg_session_params* params,
    mg_session* session) {
    if ((!params->address && !params->host) ||
        (params->address && params->host)) {
        mg_session_set_error(
            session,
            "exactly one of 'host' and 'address' parameters must be specified");
        return MG_ERROR_BAD_PARAMETER;
    }
    if ((params->username && !params->password) ||
        (!params->username && params->password)) {
        mg_session_set_error(session,
            "both username and password should be provided");
        return MG_ERROR_BAD_PARAMETER;
    }
    if ((params->sslcert && !params->sslkey) ||
        (!params->sslcert && params->sslkey)) {
        mg_session_set_error(session, "both sslcert and sslkey should be provided");
        return MG_ERROR_BAD_PARAMETER;
    }

    return 0;
}

inline static int mg_bolt_handshake(mg_session* session) {
    const uint32_t VERSION_NONE = htobe32(0);
    const uint32_t VERSION_1 = htobe32(1);
    const uint32_t VERSION_4_1 = htobe32(0x0104);
    mg_transport_suspend_until_ready_to_write(session->transport);
    if (mg_transport_send(session->transport, MG_HANDSHAKE_MAGIC,
        strlen(MG_HANDSHAKE_MAGIC)) != 0 ||
        mg_transport_send(session->transport, (char*)&VERSION_4_1, 4) != 0 ||
        mg_transport_send(session->transport, (char*)&VERSION_1, 4) != 0 ||
        mg_transport_send(session->transport, (char*)&VERSION_NONE, 4) != 0 ||
        mg_transport_send(session->transport, (char*)&VERSION_NONE, 4) != 0) {
        mg_session_set_error(session, "failed to send handshake data");
        return MG_ERROR_SEND_FAILED;
    }

    uint32_t server_version;
    mg_transport_suspend_until_ready_to_read(session->transport);
    if (mg_transport_recv(session->transport, (char*)&server_version, 4) != 0) {
        mg_session_set_error(session, "failed to receive handshake response");
        return MG_ERROR_RECV_FAILED;
    }
    if (server_version == VERSION_1) {
        session->version = 1;
    } else if (server_version == VERSION_4_1) {
        session->version = 4;
    } else {
        mg_session_set_error(session, "unsupported protocol version: %" PRIu32,
            be32toh(server_version));
        return MG_ERROR_PROTOCOL_VIOLATION;
    }
    return 0;
}

inline static mg_map* build_auth_token(const char* username, const char* password) {
    mg_map* auth_token = mg_map_make_empty(3);
    if (!auth_token) {
        return NULL;
    }

    assert((username && password) || (!username && !password));
    if (username) {
        mg_value* scheme = mg_value_make_string("basic");
        if (!scheme || mg_map_insert_unsafe(auth_token, "scheme", scheme) != 0) {
            goto cleanup;
        }

        mg_value* principal = mg_value_make_string(username);
        if (!principal ||
            mg_map_insert_unsafe(auth_token, "principal", principal)) {
            goto cleanup;
        }

        mg_value* credentials = mg_value_make_string(password);
        if (!credentials ||
            mg_map_insert_unsafe(auth_token, "credentials", credentials)) {
            goto cleanup;
        }
    } else {
        mg_value* scheme = mg_value_make_string("none");
        if (!scheme || mg_map_insert_unsafe(auth_token, "scheme", scheme) != 0) {
            goto cleanup;
        }
    }

    return auth_token;

cleanup:
    mg_map_destroy(auth_token);
    return NULL;
}

inline int handle_failure_message(mg_session* session, mg_message_failure* message) {
    int type = MG_ERROR_UNKNOWN_ERROR;
    const mg_string* code = NULL;
    const mg_string* error_message = NULL;

    {
        const mg_value* tmp = mg_map_at(message->metadata, "code");
        if (tmp && mg_value_get_type(tmp) == MG_VALUE_TYPE_STRING) {
            code = mg_value_string(tmp);
        }
        tmp = mg_map_at(message->metadata, "message");
        if (tmp && mg_value_get_type(tmp) == MG_VALUE_TYPE_STRING) {
            error_message = mg_value_string(tmp);
        }
    }

    char* type_begin;
    char* type_end;
    size_t type_size;
    if (!code) {
        goto done;
    }

    type_begin = strchr(code->data, '.');
    if (!type_begin) {
        goto done;
    }
    type_begin++;
    type_end = strchr(type_begin, '.');
    if (!type_end) {
        goto done;
    }
    type_size = (size_t)(type_end - type_begin);

    if (strncmp(type_begin, "ClientError", type_size) == 0) {
        type = MG_ERROR_CLIENT_ERROR;
    }
    if (strncmp(type_begin, "TransientError", type_size) == 0) {
        type = MG_ERROR_TRANSIENT_ERROR;
    }
    if (strncmp(type_begin, "DatabaseError", type_size) == 0) {
        type = MG_ERROR_DATABASE_ERROR;
    }

done:
    if (error_message) {
        mg_session_set_error(session, "%.*s", error_message->size,
            error_message->data);
    } else {
        mg_session_set_error(session, "unknown error occurred");
    }
    return type;
}

inline int mg_bolt_init_v1(mg_session* session, const mg_session_params* params) {
    mg_map* auth_token = build_auth_token(params->username, params->password);
    if (!auth_token) {
        return MG_ERROR_OOM;
    }

    int status =
        mg_session_send_init_message(session, params->user_agent, auth_token);
    mg_map_destroy(auth_token);

    return status;
}

inline static mg_map* build_hello_extra(const char* user_agent, const char* username,
    const char* password) {
    mg_map* extra = mg_map_make_empty(4);
    if (!extra) {
        return NULL;
    }

    if (user_agent) {
        mg_value* user_agent_value = mg_value_make_string(user_agent);
        if (!user_agent_value ||
            mg_map_insert_unsafe(extra, "user_agent", user_agent_value) != 0) {
            goto cleanup;
        }
    }

    assert((username && password) || (!username && !password));
    if (username) {
        mg_value* scheme = mg_value_make_string("basic");
        if (!scheme || mg_map_insert_unsafe(extra, "scheme", scheme) != 0) {
            goto cleanup;
        }

        mg_value* principal = mg_value_make_string(username);
        if (!principal || mg_map_insert_unsafe(extra, "principal", principal)) {
            goto cleanup;
        }

        mg_value* credentials = mg_value_make_string(password);
        if (!credentials ||
            mg_map_insert_unsafe(extra, "credentials", credentials)) {
            goto cleanup;
        }
    } else {
        mg_value* scheme = mg_value_make_string("none");
        if (!scheme || mg_map_insert_unsafe(extra, "scheme", scheme) != 0) {
            goto cleanup;
        }
    }

    return extra;

cleanup:
    mg_map_destroy(extra);
    return NULL;
}

inline int mg_bolt_init_v4(mg_session* session, const mg_session_params* params) {
    mg_map* extra =
        build_hello_extra(params->user_agent, params->username, params->password);
    if (!extra) {
        return MG_ERROR_OOM;
    }

    int status = mg_session_send_hello_message(session, extra);
    mg_map_destroy(extra);

    return status;
}

inline static int mg_bolt_init(mg_session* session, const mg_session_params* params) {
    int status = session->version == 1 ? mg_bolt_init_v1(session, params)
        : mg_bolt_init_v4(session, params);
    if (status != 0) {
        return status;
    }

    MG_RETURN_IF_FAILED(mg_session_receive_message(session));

    mg_message* response;
    MG_RETURN_IF_FAILED(mg_session_read_bolt_message(session, &response));

    if (response->type == MG_MESSAGE_TYPE_SUCCESS) {
        status = 0;
    } else if (response->type == MG_MESSAGE_TYPE_FAILURE) {
        status = handle_failure_message(session, response->failure_v);
    } else {
        status = MG_ERROR_PROTOCOL_VIOLATION;
        mg_session_set_error(session, "unexpected message type");
    }

    mg_message_destroy_ca(response, session->decoder_allocator);
    return status;
}

inline static int init_tcp_connection(const mg_session_params* params, int* sockfd,
    struct sockaddr* peer_addr,
    mg_session* session) {
    struct addrinfo* addr_list = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char portstr[6];
    sprintf(portstr, "%" PRIu16, params->port);
    int getaddrinfo_status;
    if (params->host) {
        getaddrinfo_status = getaddrinfo(params->host, portstr, &hints, &addr_list);
    } else if (params->address) {
        hints.ai_flags = AI_NUMERICHOST;
        getaddrinfo_status =
            getaddrinfo(params->address, portstr, &hints, &addr_list);
    } else {
        abort();
    }
    if (getaddrinfo_status != 0) {
#ifdef __EMSCRIPTEN__
        mg_session_set_error(session, "getaddrinfo failed: %d", getaddrinfo_status);
        // Not supported by emscripten:
        // gai_strerror(getaddrinfo_status));
#else
        mg_session_set_error(session, "getaddrinfo failed: %s",
            gai_strerror(getaddrinfo_status));
#endif
        return MG_ERROR_NETWORK_FAILURE;
    }

    int tsockfd = MG_ERROR_SOCKET;
    int status = MG_SUCCESS;
    for (struct addrinfo* curr_addr = addr_list; curr_addr;
        curr_addr = curr_addr->ai_next) {
        tsockfd = mg_socket_create(curr_addr->ai_family, curr_addr->ai_socktype,
            curr_addr->ai_protocol);
        status = mg_socket_create_handle_error(tsockfd, session);
        if (status != MG_SUCCESS) {
            continue;
        }
        status =
            mg_socket_connect(tsockfd, curr_addr->ai_addr, curr_addr->ai_addrlen);
        status = mg_socket_connect_handle_error(&tsockfd, status, session);
        if (status == MG_SUCCESS) {
            memcpy(peer_addr, curr_addr->ai_addr, sizeof(struct sockaddr));
            break;
        }
    }
    freeaddrinfo(addr_list);
    if (tsockfd == MG_ERROR_SOCKET) {
        assert(status != MG_SUCCESS);
        return status;
    }

    int set_options_status = mg_socket_options(tsockfd, session);
    if (set_options_status != MG_SUCCESS) {
        return set_options_status;
    }

    *sockfd = tsockfd;
    return 0;
}

#ifndef __EMSCRIPTEN__
inline static int get_hostname_and_ip(const struct sockaddr* peer_addr, char* hostname,
    char* ip, mg_session* session) {
    // Populate the ip.
    switch (peer_addr->sa_family) {
        case AF_INET:
            if (!inet_ntop(AF_INET, &((struct sockaddr_in*)peer_addr)->sin_addr, ip,
                INET6_ADDRSTRLEN)) {
                mg_session_set_error(session, "failed to get server IP: %s",
                    strerror(errno));
                return MG_ERROR_NETWORK_FAILURE;
            }
            break;
        case AF_INET6:
            if (!inet_ntop(AF_INET6, &((struct sockaddr_in6*)peer_addr)->sin6_addr,
                ip, INET6_ADDRSTRLEN)) {
                mg_session_set_error(session, "failed to get server IP: %s",
                    strerror(errno));
                return MG_ERROR_NETWORK_FAILURE;
            }
            break;
        default:
            // Should not happen with addresses returned from getaddrinfo.
            abort();
    }
    // Populate the hostname.
    // Useful read https://stackoverflow.com/questions/12274028.
    int nameinfo_status = getnameinfo(peer_addr, sizeof(struct sockaddr),
        hostname, NI_MAXHOST, NULL, 0, 0);
    if (nameinfo_status != 0) {
        // ON_WINDOWS getnameinfo fails if peer_addr was constructed from
        // getaddrinfo when localhost is passed in (getaddrinfo returns an empty
        // address). I haven't find simple and clean solution to make getnameinfo
        // work. Since this function is used only to get the hostname for the
        // trust callback, setting hostname to unknown and continuing the program
        // seems sensible solution.
        DB_LOG("getnameinfo call failed. hostname set to unknown\n");
        strcpy(hostname, "unknown");
    }
    return 0;
}
#endif

inline int mg_connect_ca(const mg_session_params* params, mg_session** session,
    mg_allocator* allocator) {
    // Useful read https://akkadia.org/drepper/userapi-ipv6.html.
    mg_session* tsession = mg_session_init(allocator);
    if (!tsession) {
        return MG_ERROR_OOM;
    }

    int status = 0;
    int sockfd = -1;

    status = validate_session_params(params, tsession);
    if (status != 0) {
        goto cleanup;
    }

    struct sockaddr peer_addr;
    status = init_tcp_connection(params, &sockfd, &peer_addr, tsession);
    if (status != 0) {
        goto cleanup;
    }
    switch (params->sslmode) {
        case MG_SSLMODE_DISABLE:
            status = mg_raw_transport_init(
                sockfd, (mg_raw_transport**)&tsession->transport, allocator);
            if (status != 0) {
                mg_session_set_error(tsession, "failed to initialize connection");
                goto cleanup;
            }
            break;


        case MG_SSLMODE_REQUIRE: {
#ifdef MGCLIENT_ENABLE_SSL
            mg_secure_transport* ttransport;
            status = mg_secure_transport_init(sockfd, params->sslcert, params->sslkey,
                &ttransport, allocator);
            if (status != 0) {
                mg_session_set_error(tsession,
                    "failed to initialize secure connection");
                goto cleanup;
            }
            tsession->transport = (mg_transport*)ttransport;
            if (params->trust_callback) {
                char ip[INET6_ADDRSTRLEN];
                char hostname[NI_MAXHOST];
                status = get_hostname_and_ip(&peer_addr, hostname, ip, tsession);
                if (status != 0) {
                    goto cleanup;
                }
                int trust_result = params->trust_callback(
                    hostname, ip, ttransport->peer_pubkey_type,
                    ttransport->peer_pubkey_fp, params->trust_data);
                if (trust_result != 0) {
                    mg_session_set_error(tsession,
                        "trust callback returned non-zero value");
                    status = MG_ERROR_TRUST_CALLBACK;
                    goto cleanup;
                }
            }
#else
            fprintf(stderr, "MGCLIENT_ENABLE_SSL is not defined, but sslmode is MG_SSLMODE_REQUIRE!\n");
            abort();
#endif
            break;
        }


        default:
            // Should not get here.
            abort();
    }

    // mg_transport object took ownership of the socket.
    sockfd = -1;
    status = mg_bolt_handshake(tsession);
    if (status != 0) {
        goto cleanup;
    }
    status = mg_bolt_init(tsession, params);
    if (status != 0) {
        goto cleanup;
    }

    tsession->status = MG_SESSION_READY;
    *session = tsession;
    return 0;

cleanup:
    if (sockfd >= 0 && mg_socket_close(sockfd) != 0) {
        abort();
    }
    *session = tsession;
    mg_session_invalidate(tsession);
    return status;
}

inline int mg_connect(const mg_session_params* params, mg_session** session) {
    return mg_connect_ca(params, session, &mg_system_allocator);
}

inline int handle_failure(mg_session* session) {
    int status = 0;
    status = session->version == 1 ? mg_session_send_ack_failure_message(session)
        : mg_session_send_reset_message(session);
    if (status != 0) {
        return status;
    }

    status = mg_session_receive_message(session);
    if (status != 0) {
        return status;
    }

    mg_message* response;
    status = mg_session_read_bolt_message(session, &response);
    if (status != 0) {
        return status;
    }

    if (response->type != MG_MESSAGE_TYPE_SUCCESS) {
        status = MG_ERROR_PROTOCOL_VIOLATION;
        mg_session_set_error(session, "unexpected message type");
    }

    mg_message_destroy_ca(response, session->decoder_allocator);
    return status;
}

inline int mg_session_run(mg_session* session, const char* query, const mg_map* params,
    const mg_map* extra_run_information, const mg_list** columns,
    int64_t* qid) {
    if (session->status == MG_SESSION_BAD) {
        mg_session_set_error(session, "bad session");
        return MG_ERROR_BAD_CALL;
    }
    if (!session->explicit_transaction &&
        session->status == MG_SESSION_EXECUTING) {
        mg_session_set_error(session, "already executing a query");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_FETCHING) {
        mg_session_set_error(session, "fetching results of a query");
        return MG_ERROR_BAD_CALL;
    }

    assert(session->status == MG_SESSION_READY ||
        (session->version == 4 && session->explicit_transaction &&
            session->status == MG_SESSION_EXECUTING));

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;
    mg_list_destroy_ca(session->result.columns, session->allocator);
    session->result.columns = NULL;

    if (!params) {
        params = &mg_empty_map;
    }

    // extra field allowed only allowed for Auto-commit Transaction
    // TODO(aandelic): Check if sending extra run information while in Explicit
    // Transaction should result with an error
    if (session->version == 4 &&
        (!extra_run_information || session->explicit_transaction)) {
        extra_run_information = &mg_empty_map;
    }

    int status = 0;
    status = mg_session_send_run_message(session, query, params,
        extra_run_information);
    if (status != 0) {
        goto fatal_failure;
    }

    mg_transport_suspend_until_ready_to_read(session->transport);
    status = mg_session_receive_message(session);
    if (status != 0) {
        goto fatal_failure;
    }

    mg_message* response;

    status = mg_session_read_bolt_message(session, &response);
    if (status != 0) {
        goto fatal_failure;
    }

    if (response->type == MG_MESSAGE_TYPE_SUCCESS) {
        const mg_value* columns_tmp =
            mg_map_at(response->success_v->metadata, "fields");
        if (!columns_tmp || mg_value_get_type(columns_tmp) != MG_VALUE_TYPE_LIST) {
            status = MG_ERROR_PROTOCOL_VIOLATION;
            mg_message_destroy_ca(response, session->decoder_allocator);
            mg_session_set_error(session, "invalid response metadata");
            goto fatal_failure;
        }
        session->result.columns =
            mg_list_copy_ca(mg_value_list(columns_tmp), session->allocator);
        mg_message_destroy_ca(response, session->decoder_allocator);
        if (!session->result.columns) {
            mg_session_set_error(session, "out of memory");
            return MG_ERROR_OOM;
        }

        if (session->version == 4 && session->explicit_transaction) {
            if (qid) {
                const mg_value* qid_tmp =
                    mg_map_at(response->success_v->metadata, "qid");

                if (!qid_tmp || mg_value_get_type(qid_tmp) != MG_VALUE_TYPE_INTEGER) {
                    status = MG_ERROR_PROTOCOL_VIOLATION;
                    mg_message_destroy_ca(response, session->decoder_allocator);
                    mg_session_set_error(session, "invalid response metadata");
                    goto fatal_failure;
                }

                *qid = mg_value_integer(qid_tmp);
            }

            ++session->query_number;
        }

        if (columns) {
            *columns = session->result.columns;
        }

        session->status = MG_SESSION_EXECUTING;
        return 0;
    }

    if (response->type == MG_MESSAGE_TYPE_FAILURE) {
        int failure_status = handle_failure_message(session, response->failure_v);

        status = handle_failure(session);
        if (status != 0) {
            goto fatal_failure;
        }

        mg_message_destroy_ca(response, session->decoder_allocator);
        return failure_status;
    }

    status = MG_ERROR_PROTOCOL_VIOLATION;
    mg_message_destroy_ca(response, session->decoder_allocator);
    mg_session_set_error(session, "unexpected message type");

fatal_failure:
    mg_session_invalidate(session);
    assert(status != 0);
    return status;
}

inline int mg_session_pull(mg_session* session, const mg_map* pull_information) {
    if (session->status == MG_SESSION_BAD) {
        mg_session_set_error(session, "called pull while bad session");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_READY) {
        mg_session_set_error(session, "called pull while not executing a query");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_FETCHING) {
        mg_session_set_error(session, "called pull while still fetching data");
        return MG_ERROR_BAD_CALL;
    }

    assert(session->status == MG_SESSION_EXECUTING);

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;

    int status = 0;
    if (session->version == 4 && !pull_information) {
        pull_information = mg_default_pull_extra_map;
    }

    status = mg_session_send_pull_message(session, pull_information);
    if (status != 0) {
        goto fatal_failure;
    }

    session->status = MG_SESSION_FETCHING;
    return 0;

fatal_failure:
    mg_session_invalidate(session);
    assert(status != 0);
    return status;
}

inline int mg_session_fetch(mg_session* session, mg_result** result) {
    if (session->status == MG_SESSION_BAD) {
        mg_session_set_error(session, "called fetch while bad session");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_READY) {
        mg_session_set_error(session, "called fetch while not executing a query");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_EXECUTING) {
        mg_session_set_error(session, "called fetch without pulling results");
        return MG_ERROR_BAD_CALL;
    }
    assert(session->status == MG_SESSION_FETCHING);

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;

    int status = 0;

    mg_message* message = NULL;
    status = mg_session_receive_message(session);
    if (status != 0) {
        goto fatal_failure;
    }

    status = mg_session_read_bolt_message(session, &message);
    if (status != 0) {
        goto fatal_failure;
    }

    if (message->type == MG_MESSAGE_TYPE_RECORD) {
        session->result.message = message;
        *result = &session->result;
        return 1;
    }

    if (message->type == MG_MESSAGE_TYPE_SUCCESS) {
        if (session->version == 4) {
            const mg_value* has_more =
                mg_map_at(message->success_v->metadata, "has_more");

            if (has_more && has_more->type != MG_VALUE_TYPE_BOOL) {
                status = MG_ERROR_PROTOCOL_VIOLATION;
                mg_message_destroy_ca(message, session->decoder_allocator);
                mg_session_set_error(session, "invalid response metadata");
                goto fatal_failure;
            }

            if (!has_more || !mg_value_bool(has_more)) {
                session->query_number -= session->explicit_transaction;
                session->status = session->explicit_transaction && session->query_number
                    ? MG_SESSION_EXECUTING
                    : MG_SESSION_READY;
            } else {
                session->status = MG_SESSION_EXECUTING;
            }
        } else {
            session->status = MG_SESSION_READY;
        }
        session->result.message = message;
        *result = &session->result;
        return 0;
    }

    if (message->type == MG_MESSAGE_TYPE_FAILURE) {
        int failure_status = handle_failure_message(session, message->failure_v);
        mg_message_destroy_ca(message, session->decoder_allocator);

        status = handle_failure(session);
        if (status != 0) {
            goto fatal_failure;
        }

        mg_message_destroy_ca(message, session->decoder_allocator);
        session->status = MG_SESSION_READY;
        return failure_status;
    }

    status = MG_ERROR_PROTOCOL_VIOLATION;
    mg_session_set_error(session, "unexpected message type");
    mg_message_destroy_ca(message, session->decoder_allocator);

fatal_failure:
    mg_session_invalidate(session);
    return status;
}

inline int mg_session_begin_transaction(mg_session* session,
    const mg_map* extra_run_information) {
    if (session->version == 1) {
        mg_session_set_error(session,
            "Transaction are not supported in this version");
    }
    if (session->status == MG_SESSION_BAD) {
        mg_session_set_error(session, "bad session");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_EXECUTING) {
        mg_session_set_error(
            session, "Cannot start a transaction while a query is executing");
        return MG_ERROR_BAD_CALL;
    }
    if (session->status == MG_SESSION_FETCHING) {
        mg_session_set_error(session, "fetching result of a query");
        return MG_ERROR_BAD_CALL;
    }
    if (session->explicit_transaction) {
        mg_session_set_error(session, "Transaction already started");
        return MG_ERROR_BAD_CALL;
    }
    assert(session->status == MG_SESSION_READY && !session->explicit_transaction);

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;
    // TODO(aandelic): Check if the columns should be destroyed

    if (!extra_run_information) {
        extra_run_information = &mg_empty_map;
    }

    int status = 0;
    status = mg_session_send_begin_message(session, extra_run_information);
    if (status != 0) {
        goto fatal_failure;
    }

    status = mg_session_receive_message(session);
    if (status != 0) {
        goto fatal_failure;
    }

    mg_message* response;
    status = mg_session_read_bolt_message(session, &response);
    if (status != 0) {
        goto fatal_failure;
    }

    if (response->type == MG_MESSAGE_TYPE_SUCCESS) {
        mg_message_destroy_ca(response, session->decoder_allocator);
        session->explicit_transaction = 1;
        session->query_number = 0;
        return 0;
    }

    if (response->type == MG_MESSAGE_TYPE_FAILURE) {
        int failure_status = handle_failure_message(session, response->failure_v);

        status = handle_failure(session);
        if (status != 0) {
            goto fatal_failure;
        }

        mg_message_destroy_ca(response, session->decoder_allocator);
        return failure_status;
    }

    status = MG_ERROR_PROTOCOL_VIOLATION;
    mg_message_destroy_ca(response, session->decoder_allocator);
    mg_session_set_error(session, "unexpected message type");

fatal_failure:
    mg_session_invalidate(session);
    assert(status != 0);
    return status;
}

inline int mg_session_end_transaction(mg_session* session, int commit_transaction,
    mg_result** result) {
    if (session->version == 1) {
        mg_session_set_error(session,
            "Transaction are not supported in this version");
    }
    if (session->status == MG_SESSION_BAD) {
        mg_session_set_error(session, "bad session");
        return MG_ERROR_BAD_CALL;
    }

    if (!session->explicit_transaction) {
        mg_session_set_error(session, "No active transaction");
        return MG_ERROR_BAD_CALL;
    }

    if (session->status == MG_SESSION_EXECUTING ||
        session->status == MG_SESSION_FETCHING) {
        mg_session_set_error(session,
            "Cannot end a transaction while a query is executing");
        return MG_ERROR_BAD_CALL;
    }

    assert(session->status == MG_SESSION_READY && session->explicit_transaction);

    mg_message_destroy_ca(session->result.message, session->decoder_allocator);
    session->result.message = NULL;
    // TODO(aandelic): Check if the columns should be destroyed

    int status = 0;
    status = commit_transaction ? mg_session_send_commit_messsage(session)
        : mg_session_send_rollback_messsage(session);
    if (status != 0) {
        goto fatal_failure;
    }

    status = mg_session_receive_message(session);
    if (status != 0) {
        goto fatal_failure;
    }

    mg_message* response;

    status = mg_session_read_bolt_message(session, &response);
    if (status != 0) {
        goto fatal_failure;
    }

    if (response->type == MG_MESSAGE_TYPE_SUCCESS) {
        session->result.message = response;
        *result = &session->result;
        session->status = MG_SESSION_READY;
        session->explicit_transaction = 0;
        return 0;
    }

    if (response->type == MG_MESSAGE_TYPE_FAILURE) {
        int failure_status = handle_failure_message(session, response->failure_v);

        status = handle_failure(session);
        if (status != 0) {
            goto fatal_failure;
        }

        mg_message_destroy_ca(response, session->decoder_allocator);
        return failure_status;
    }

    status = MG_ERROR_PROTOCOL_VIOLATION;
    mg_message_destroy_ca(response, session->decoder_allocator);
    mg_session_set_error(session, "unexpected message type");

fatal_failure:
    mg_session_invalidate(session);
    assert(status != 0);
    return status;
}

inline int mg_session_commit_transaction(mg_session* session, mg_result** result) {
    return mg_session_end_transaction(session, 1, result);
}

inline int mg_session_rollback_transaction(mg_session* session, mg_result** result) {
    return mg_session_end_transaction(session, 0, result);
}

inline const mg_list* mg_result_columns(const mg_result* result) {
    return result->columns;
}

inline const mg_list* mg_result_row(const mg_result* result) {
    if (!result->message) {
        return NULL;
    }
    if (result->message->type != MG_MESSAGE_TYPE_RECORD) {
        return NULL;
    }
    return result->message->record_v->fields;
}

inline const mg_map* mg_result_summary(const mg_result* result) {
    if (!result->message) {
        return NULL;
    }
    if (result->message->type != MG_MESSAGE_TYPE_SUCCESS) {
        return NULL;
    }
    return result->message->success_v->metadata;
}


#endif // MGCLIENT_H