#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "xdpw.h"

static const char object_path[] = "/org/freedesktop/portal/desktop";
static const char interface_name[] = "org.freedesktop.impl.portal.ScreenCast";

static int sd_method_create_session(sd_bus_message *msg, void *data, sd_bus_error *ret_error) {
	TRACE;
	char *handle, *session_handle, *app_id;

	sd_bus_message_read(msg, "oos", &handle, &session_handle, &app_id);
	request_create(sd_bus_message_get_bus(msg), handle);

	return send_portal_response(msg, PORTAL_RESPONSE_SUCCESS, 0);
}

static int sd_method_select_sources(sd_bus_message *msg, void *data, sd_bus_error *ret_error) {
	TRACE;
	char *handle, *session_handle, *app_id;

	sd_bus_message_read(msg, "oos", &handle, &session_handle, &app_id);
	/*sd_bus_message_read(msg, "a{sv}", &options_length, &options);*/
	request_create(sd_bus_message_get_bus(msg), handle);

	return send_portal_response(msg, PORTAL_RESPONSE_SUCCESS, 0);
}

static int sd_method_start(sd_bus_message *msg, void *data, sd_bus_error *ret_error) {
	TRACE;
	char *handle, *session_handle, *app_id, *parent_window;

	sd_bus_message_read(msg, "ooss", &handle, &session_handle, &app_id, &parent_window);
	/*sd_bus_message_read(msg, "a{sv}", &options_length, &options);*/
	request_create(sd_bus_message_get_bus(msg), handle);

	return send_portal_response(msg, PORTAL_RESPONSE_SUCCESS, 1, "streams", "a(ua{sv})", 0);
}

static const sd_bus_vtable screencast_vtable[] = {
  SD_BUS_VTABLE_START(0),
  SD_BUS_METHOD("CreateSession", "oosa{sv}", "ua{sv}", sd_method_create_session, SD_BUS_VTABLE_UNPRIVILEGED),
  SD_BUS_METHOD("SelectSources", "oosa{sv}", "ua{sv}", sd_method_select_sources, SD_BUS_VTABLE_UNPRIVILEGED),
  SD_BUS_METHOD("Start", "oossa{sv}", "ua{sv}", sd_method_start, SD_BUS_VTABLE_UNPRIVILEGED),
  SD_BUS_VTABLE_END
};

int init_screencast(sd_bus *bus) {
	// TODO: cleanup
	sd_bus_slot *slot = NULL;
	return sd_bus_add_object_vtable(bus, &slot, object_path, interface_name,
		screencast_vtable, NULL);
}
