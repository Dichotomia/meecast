/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
	
*/
#include "weather-home_download.h"


/* Create standard Hildon animation small window */
void create_window_update(){
    update_window = hildon_banner_show_animation(app->top_widget,
						    NULL,
						    _("Update weather"));
    g_object_ref(G_OBJECT(update_window));
}


/* Callback function for request  connection to Internet */
void iap_callback(struct iap_event_t *event, void *arg){
    switch(event->type){
	case OSSO_IAP_CONNECTED:
	    second_attempt = TRUE;
	    update_weather();
	break;
	case OSSO_IAP_DISCONNECTED:
	break;
	case OSSO_IAP_ERROR:
	    hildon_banner_show_information(app->main_window,
					    NULL,
					    _("Not connected to Internet"));
	break;
    }
}
/* Check connect to Internet and connection if it not */
gboolean get_connected(void){
    /* Register a callback function for IAP related events. */
    if(osso_iap_cb(iap_callback) != OSSO_OK)
	return FALSE;
    if(osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK)
	return FALSE;
    return TRUE;
}


/* Init easy curl */
CURL* weather_curl_init(void){
    CURL *curl_handle;
    curl_handle = curl_easy_init(); 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, 
            "Mozilla/5.0 (X11; U; Linux i686; en-US; " 
            "rv:1.8.1.1) Gecko/20061205 Iceweasel/2.0.0.1"); 
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30); 
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10); 
    config_update_proxy();
    /* Set Proxy option */
    if(app->iap_http_proxy_host){ 
        curl_easy_setopt(curl_handle, CURLOPT_PROXY, app->iap_http_proxy_host); 
        if(app->iap_http_proxy_port) 
            curl_easy_setopt(curl_handle, CURLOPT_PROXYPORT, app->iap_http_proxy_port); 
    } 
    return curl_handle;    
}

int data_read(void *buffer, size_t size, size_t nmemb, void *stream){
    int result;
    struct HtmlFile *out=(struct HtmlFile *)stream;

    if(out && !out->stream){
    /* open file for writing */
	out->stream = fopen(out->filename, "wb");
	if(!out->stream)
	    return -1; /* failure, can't open file to write */      
    }
    result = fwrite(buffer, size, nmemb, out->stream);
    return result;
}			  

/* Form URL and filename for  write xml file. 
   Returns TRUE if the station is taken from the list
   Else return FLASE. This the end list
*/
gboolean form_url_and_filename(){
    if(tmplist != NULL){
        ws = tmplist->data;
        if(ws->id_station != NULL){
	    if(url){
		g_string_free(url, TRUE);    
		url = NULL;
	    } 
	    if(full_filename_new_xml){
	        g_string_free(full_filename_new_xml, TRUE);    
		full_filename_new_xml = NULL;
	    } 
	    url = g_string_new(NULL);        
    	    g_string_append_printf(url,"http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=%d",
				    ws->id_station, DAY_DOWNLOAD);
	    full_filename_new_xml = g_string_new(NULL);        
	    g_string_append_printf(full_filename_new_xml,"%s/%s.xml.new",
				    app->_weather_dir_name, ws->id_station);
	    tmplist = g_slist_next(tmplist);
	    /* Forming structure for download data of weather */
	    html_file.filename = full_filename_new_xml->str;
    	    html_file.stream = NULL;
	    return TRUE;
	}
	else
	    return FALSE;	
    }
    else
	return FALSE;
}

/* Download html/xml file. Call every 100 ms after begin download*/
gboolean download_html(gpointer data){

    CURLMsg *msg;

    if(app->popup_window){
	gtk_widget_destroy(app->popup_window);   
        app->popup_window=NULL;
    }
    
    /* For emulator only for update forecast*/
    second_attempt = TRUE;    
    if( app->show_update_window && (!second_attempt) ){
        get_connected();        
        return FALSE;
    }
    second_attempt = FALSE;
    /* The second stage */
    /* call curl_multi_perform for read weather data from Inet */
    if(curl_multi && CURLM_CALL_MULTI_PERFORM
            == curl_multi_perform(curl_multi, &num_transfers))
        return TRUE; /* return to UI */

    /* The first stage */
    if(!curl_handle){
	if(app->show_update_window)
    	    create_window_update(); /* Window with update information */
        /* Initialize list */
        tmplist = stations_view_list;
	if(!form_url_and_filename()){
	    if(url){
		g_string_free(url, TRUE);    
		url = NULL;
	    }	 
	    if(full_filename_new_xml){
	        g_string_free(full_filename_new_xml, TRUE);    
		full_filename_new_xml = NULL;
	    }	 
	    return FALSE; /* The strange error */		
	}    
	/* Init easy_curl */
	curl_handle = weather_curl_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
	/* Init curl_mult */
	if(!curl_multi)
    	    curl_multi = curl_multi_init();
	/* set options for the curl easy handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);		
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);	
        /* for debug */
        /*    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1"); */
        /* add the easy handle to a multi session */
        curl_multi_add_handle(curl_multi, curl_handle);	
        return TRUE; /* return to UI */
    }
    else{
        /* The third stage */
	while(curl_multi && (msg = curl_multi_info_read(curl_multi, &num_msgs))){
	    if(msg->msg == CURLMSG_DONE){	  
		if(msg->data.result != CURLE_OK){ /* Not success of the download */
		    if(app->show_update_window)
			hildon_banner_show_information(app->main_window, 
							NULL,
							_("Did not download weather"));
		}
		else{ /* Clean */
		    curl_multi_remove_handle(curl_multi,msg->easy_handle); /* Delete curl_handle from curl_multi */
		    if(html_file.stream)
        		fclose(html_file.stream);
		    if(!form_url_and_filename()){ /* Success - all is downloaded */
			if(app->show_update_window)
			    hildon_banner_show_information(app->main_window,
							    NULL,
							    _("Weather updated"));	    
        		weather_frame_update(FALSE);
		    }
		    else{
			/* set options for the curl easy handle */
			curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
        		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);		
        		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);	
    			/* add the easy handle to a multi session */
    			curl_multi_add_handle(curl_multi, curl_handle);	
			return TRUE;/* Download next station */
		    }		    
		}
		if(update_window){
		    gtk_widget_destroy(update_window);
		    update_window = NULL;
		}
	  /* Clean all */    
    		curl_multi_remove_handle(curl_multi,msg->easy_handle);
		curl_multi_cleanup(curl_multi);
		curl_multi = NULL;
		curl_handle = NULL;
		if(url){
		    g_string_free(url, TRUE);    
		    url = NULL;
		}	 
		if(full_filename_new_xml){
		    g_string_free(full_filename_new_xml, TRUE);    	  
		    full_filename_new_xml = NULL;
		}     
		return FALSE; /* This is the end */
	    }
	}
	return TRUE;
    }
    return FALSE;
}
