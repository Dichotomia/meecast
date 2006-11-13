/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
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

#include "weather-home_preference.h"





/* Free memory allocated for stations list */
void free_list_stations (void)
{
 static GSList *stations_list_temp = NULL;
 struct station_and_weather_code *sc;

 if (stations_list_in_state != NULL)
 { 
  stations_list_temp = stations_list_in_state; 
  while (stations_list_temp != NULL)
  {
     sc = stations_list_temp->data;
     g_free(sc->station_name);
     g_free(sc->station_code);
     g_free(sc);
     stations_list_temp = g_slist_next(stations_list_temp);
  }
  g_slist_free(stations_list_in_state);
  stations_list_in_state = NULL;
 } 
}

/* Select item on country combobox */
void changed_country(void)
{
  GtkTreeModel *model;
  char flag; //Flag for country processing
  char flag_new_state; //Flag for new country or province or state
  unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
  static gchar *gcountry_name = NULL;
  FILE *stations_file, *iso3166_file; 
  char country_name[52];
  char country_code [2];
  char temp_state_name[20];
  int count_state = 0; // Count state of file iso3166 

  /* Search Country defined ComboBox in iso file */
  flag = FALSE;    
  /* Clear the list. */
  model = gtk_combo_box_get_model (GTK_COMBO_BOX (states));
  gtk_list_store_clear (GTK_LIST_STORE (model));
  model = gtk_combo_box_get_model (GTK_COMBO_BOX (stations));
  gtk_list_store_clear (GTK_LIST_STORE (model));

  gcountry_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(countrys));
    /* Search country code */    
    if ((iso3166_file = fopen(COUNTRYS_FILE,"r")) != NULL )
    {
     while(!feof(iso3166_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), iso3166_file);//Read Next Line
      if (strlen(out_buffer)>0){
       if (strcmp("----------------------------------------------------------------------\n",out_buffer) == 0)
       {
         flag = (flag == TRUE) ? FALSE : TRUE ;
       } 	
       else
       if ( flag == TRUE)
       {
         if (strcmp("\n",out_buffer) != 0)
	 {
          sprintf(country_name,"%.38s",out_buffer);
	  if (strcmp(gcountry_name,country_name) == 0)
	  {
	    /* Write country code */
	    country_code[0]=out_buffer[48];
	    country_code[1]=out_buffer[49];	    
	    country_code[2]=0;
	  }    	  
	 } 
       }
      }	
     }
    }
    
    /* Search state or province on country and add it to combobox state */
    if((stations_file = fopen(STATIONS_FILE,"r")) != NULL)
    {
     memset(temp_state_name, 0, sizeof(temp_state_name)); //Clear temporary value
     flag_new_state = FALSE;
     while(!feof(stations_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), stations_file);//Read Next Line
      if ( (strlen(out_buffer)>0) && ((char)out_buffer[0] != '!') )
      {   
        /* Is it country or state or province name ? */
    	if ( out_buffer[19] != ' ' )
	{
	  sprintf(temp_state_name,"%.19s",out_buffer);
	  flag_new_state = TRUE;
	}  
	if ( (flag_new_state == TRUE) && country_code[0] == out_buffer[81] &&  country_code[1] == out_buffer[82])
	{
	    flag_new_state = FALSE;	
            gtk_combo_box_append_text(GTK_COMBO_BOX(states), temp_state_name);
	     count_state++;
	     if ((_weather_state_name != NULL) &&(strcmp(temp_state_name,_weather_state_name) == 0))
	     {

	      index_state = count_state;
	     }
	}    	  
      } 
     }
    }    
 g_free(_weather_country_name);    
 _weather_country_name = gcountry_name;
 free_list_stations();
}

/* Select item on state combobox */
void changed_state(void)
{
  GtkTreeModel *model;
  char flag; //Flag for country processing
  char flag_necessary_state; //Flag for finding country or province or state
  unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
  static gchar *gstate_name = NULL;
  FILE *stations_file; 
  char state_name[21];
  char temp_station_name[21];
  char temp_station_code[9];
  struct station_and_weather_code *sc;
  int count_station = 0; // Count station of state or region
  int i;
  

 /* Search Country in the ComboBox*/
    flag = FALSE;    
    /* Clear the list. */
    model = gtk_combo_box_get_model (GTK_COMBO_BOX (stations));
    gtk_list_store_clear (GTK_LIST_STORE (model));

    gstate_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(states));
    /* Search state or province on country and add stations to combobox*/
    if((stations_file = fopen(STATIONS_FILE,"r")) != NULL)
    {
     memset(state_name, 0, sizeof(state_name)); //Clear temporary value
     flag_necessary_state = FALSE;
     while(!feof(stations_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), stations_file);//Read Next Line
      memset(temp_station_name, 0, sizeof(temp_station_name)); //Clear buffer
      if ( (strlen(out_buffer)>27) && ((char)out_buffer[0] != '!') )
      {  
        /* Is it country or state or province name ? */
    	if ( out_buffer[19] != ' ' )
	{
	  sprintf(state_name,"%.19s",out_buffer);
	  flag_necessary_state = (strcmp(state_name,gstate_name) == 0) ? TRUE : FALSE;
	} 

	/* Check for wrong string */ 
	if ((flag_necessary_state == TRUE)&& (strlen(out_buffer)>90) )
	{
	  /* Prepare strings station_name and code_name for work */
	  for (i=3;i<19;i++) temp_station_name[i-3] = out_buffer[i];
	  for (i=84;i<92;i++) temp_station_code[i-84] = out_buffer[i];
	  temp_station_code[8]=0;
	   /* Copy name station to combobox */
	  gtk_combo_box_append_text(GTK_COMBO_BOX(stations), temp_station_name);


          count_station++;
	  if ((_weather_station_name != NULL) &&(strcmp(temp_station_name,_weather_station_name) == 0))
	  {    
	    index_station = count_station;
          }
	
	  /* Add station and station code to list */	  
	  sc = g_new0(struct station_and_weather_code, 1);
	  sc->station_name = g_strdup(temp_station_name);	  
	  sc->station_code = g_strdup(temp_station_code);	  
	  stations_list_in_state = g_slist_append(stations_list_in_state, sc); /* Necessary free list  beyond !!! */	  
	}    	  
      } 
     }
    }    
}

/* Select item on station combobox */
void changed_stations(void)
{
  struct station_and_weather_code *sc;
  static GSList *stations_list_temp = NULL;
  
  stations_list_temp = stations_list_in_state;
  while (stations_list_temp != NULL)
  {
   sc = stations_list_temp->data;
   if (strcmp(sc->station_name,g_strdup(gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations)))) == 0)
   {
    if (_weather_station_id != NULL) g_free(_weather_station_id);
    _weather_station_id = g_strdup(sc->station_code);
    break;    
   }
   stations_list_temp = g_slist_next(stations_list_temp);
  }
}

/* Fill station list (tree) */
void
fill_station_list_view (GtkWidget *station_list_view,GtkWidget *station_list_store)
{
 GSList *tmplist = NULL; //Temorary for station list
 struct weather_station *ws; //Description Weather station
 GtkTreeSelection *list_selection;
 GtkTreeIter iter;

 list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
 tmplist = stations_view_list;
 while (tmplist != NULL)
 {
     ws = tmplist->data;
     gtk_list_store_append(GTK_LIST_STORE
                              (station_list_store), &iter);
     gtk_list_store_set(GTK_LIST_STORE(station_list_store),
                            &iter,
                            0, ws->name_station,-1);
     if (strcmp(_weather_station_id, ws->id_station)== 0)
        gtk_tree_selection_select_iter(list_selection,&iter);
     tmplist = g_slist_next(tmplist);
 }
}



/* Delete station from list */
static gboolean
delete_station (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
 GSList *tmplist = NULL; //Temorary for station list
 struct weather_station *ws; //Description Weather station
 GtkTreeIter iter;
 gchar *station_selected = NULL;
  
 GtkTreeModel *model = gtk_tree_view_get_model(station_list_view);
 GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view)); 
 if ( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
  return FALSE;
 
 gtk_tree_model_get(model, &iter, 0, &station_selected,-1); 
 
 tmplist = stations_view_list;
 while (tmplist != NULL)
 {
     ws = tmplist->data;
     if (strcmp(station_selected, ws->name_station)== 0)
     {
      /* Remove station from the Station List */
      stations_view_list = g_slist_remove(stations_view_list, ws);
      /* Update config file */
      config_save();
      gtk_list_store_clear(station_list_store);
      fill_station_list_view (station_list_view,station_list_store);
      /* Update station list */
      flag_update_station = TRUE;
      model = gtk_tree_view_get_model(station_list_view);
      selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));

      /* Search new selected station */    
      if ( gtk_tree_selection_get_selected(selection, NULL, &iter) )
      {
       gtk_tree_model_get(model, &iter, 0, &station_selected,-1);
       tmplist = stations_view_list;
       while (tmplist != NULL)
       {
        ws = tmplist->data;
        if (strcmp(station_selected, ws->name_station)== 0)
        {
         /* Set New selected station on default on main display*/
         if (_weather_station_id != NULL) g_free(_weather_station_id);
         _weather_station_id = g_strdup(ws->id_station); 
        }
	tmplist = g_slist_next(tmplist);
       }
      }
      else
      {
       _weather_station_id = NULL;
      } 
      break; 
     }
     tmplist = g_slist_next(tmplist);
 }
 return TRUE;
}

static GtkListStore *
create_station_list_store(void)
{
  GtkListStore *station_list = NULL;
  station_list = gtk_list_store_new(1,G_TYPE_STRING);
  return station_list;
}


static GtkWidget *
create_tree_view(GtkListStore * list)
{
  GtkWidget *tree_view = NULL;
  GtkTreeSelection *list_selection = NULL;
  GtkCellRenderer *list_renderer = NULL;
  GtkTreeViewColumn *list_column = NULL;

  /* create the tree view model LIST */
  tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list));

  /* make the list component single selectable */
  list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  gtk_tree_selection_set_mode(list_selection, GTK_SELECTION_SINGLE);

  /* add name column to the view */
  list_renderer = gtk_cell_renderer_text_new();
  list_column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_sizing(list_column, GTK_TREE_VIEW_COLUMN_FIXED);
  g_object_set(G_OBJECT(list_renderer),
               "ellipsize", PANGO_ELLIPSIZE_END, NULL);
  gtk_tree_view_column_set_expand(list_column, TRUE);

  gtk_tree_view_column_pack_start(list_column, list_renderer, TRUE);
  gtk_tree_view_column_set_attributes(list_column, list_renderer,
                                      "text", 0, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), list_column);
  /* return widget to caller */
  return tree_view;
}

void
weather_window_add_station (GtkWidget *widget,
                               GdkEvent *event,
                           gpointer user_data)
{
  FILE  *iso3166_file;
  char country_name[52];
  unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
  char flag; //Flag for country processing
  int count_country = 0; // Count country of file iso3166 
  int index_country = 0; // Position country of the list 
  struct weather_station *ws; //Temp struct for station

  GtkWidget *window_add_station;
  GtkWidget *label;
  GtkWidget *table;

  /* Create dialog window */
  window_add_station = gtk_dialog_new_with_buttons("Add Station",
            NULL, GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            NULL);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_add_station)->vbox),
            table = gtk_table_new(4, 2, FALSE), TRUE, TRUE, 0);
  gtk_dialog_add_button(GTK_DIALOG(window_add_station),
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
 
  /* Add Country */
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new("Country:"),
            0, 1, 0, 1);
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 0, 1);
  gtk_container_add(GTK_CONTAINER(label),countrys = gtk_combo_box_new_text());

  /* Add State */
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new("State(Province):"),
            0, 1, 2, 3);
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 2, 3);
  gtk_container_add(GTK_CONTAINER(label),states = gtk_combo_box_new_text());
  
  /* Add Station */
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new("Station(Place):"),
            0, 1, 3, 4);
  gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 3, 4);
  gtk_container_add(GTK_CONTAINER(label),stations = gtk_combo_box_new_text());

  gtk_widget_show_all(window_add_station);
   
  /* Inserting Countrys to ComboBox from iso file*/
    flag = FALSE;
    if((iso3166_file = fopen(COUNTRYS_FILE,"r")) != NULL)
    {
     while(!feof(iso3166_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), iso3166_file);//Read Next Line
      if (strlen(out_buffer)>0){
       if (strcmp("----------------------------------------------------------------------\n",out_buffer) == 0)
       {
        flag = (flag == TRUE) ? FALSE : TRUE ;
       }
       else
       if ( flag == TRUE)
       {
         if (strcmp("\n",out_buffer) != 0)
         {
          sprintf(country_name,"%.38s",out_buffer);
          gtk_combo_box_append_text(GTK_COMBO_BOX(countrys), country_name);
          count_country++;
          if ((_weather_country_name != NULL) &&(strcmp(country_name,_weather_country_name) == 0))
          {
           index_country = count_country;
          }
         }
       }
      }
     }
     fclose(iso3166_file);
    }

  /* Set default value to country combo_box */
  if (index_country != 0)
  {
     gtk_combo_box_set_active (GTK_COMBO_BOX(countrys),index_country-1);
     changed_country();
  }

  g_signal_connect ((gpointer) countrys, "changed",
                    G_CALLBACK (changed_country),
                    NULL);
  g_signal_connect ((gpointer) states, "changed",
                    G_CALLBACK (changed_state),
                    NULL);
  g_signal_connect ((gpointer) stations, "changed",
                    G_CALLBACK (changed_stations),
                    NULL);

  while(GTK_RESPONSE_ACCEPT == gtk_dialog_run(GTK_DIALOG(window_add_station)) &&
        gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations)) != NULL) /* Press Button Ok and not null 
									   station combobox */
  {
    flag_update_station = TRUE;
    ws = g_new0(struct weather_station,1);
    ws->id_station = g_strdup(_weather_station_id);
    if (_weather_station_name != NULL) g_free(_weather_station_name);
    _weather_station_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));
    ws->name_station = g_strdup(_weather_station_name);
    /* Add station to stations list */
    stations_view_list = g_slist_append(stations_view_list, ws); 
    /* Update config file */
    config_save();
    /* Add station to View List(Tree) */
    gtk_list_store_clear(station_list_store);
    fill_station_list_view (station_list_view,station_list_store);
    break;
  }
  
  gtk_widget_destroy(window_add_station);
}

/* Main preference window */
void
weather_window_preference (GtkWidget *widget,
                               GdkEvent *event,
                           gpointer user_data)
{

   GtkWidget *window_config, *notebook; 
   GtkWidget *label;
   GtkWidget *table;
   GtkWidget *font_color;
   GtkWidget *chk_transparency;
   GtkTreeViewColumn *col;
   GtkCellRenderer  *renderer;
   GtkWidget *scrolled_window;
   GtkWidget *button_add, *button_del;

   char flag_update_icon; //Flag update main weather icon of desktop 
   int index_update_time = 0; // Position acive update time of the list 
   GdkColor _weather_font_color_temp; // Temporary for font color
   static GSList *time_update_list_temp = NULL; //Temporary list for time update
   struct time_update *tu; //Temporary for time update list
   
   not_event = TRUE;
   flag_update_station = FALSE;
   flag_update_icon = FALSE;

   window_config = gtk_dialog_new_with_buttons("Other Maemo Weather Settings",
            NULL, GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            NULL);
    gtk_dialog_add_button(GTK_DIALOG(window_config),
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);

    /* Create Notebook widget */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_config)->vbox),
            notebook = gtk_notebook_new(), TRUE, TRUE, 0);

    /* Location page. */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
            table = gtk_table_new(4, 2, FALSE),
            label = gtk_label_new("Location"));
	    
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            0, 1, 0, 6);
  
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(scrolled_window,GTK_SHADOW_OUT);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 300, 180);


    station_list_store = create_station_list_store();
    station_list_view = create_tree_view(station_list_store);
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                    GTK_WIDGET(station_list_view));
    fill_station_list_view (station_list_view,station_list_store);
    gtk_container_add(GTK_CONTAINER(label), scrolled_window);
 
    gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new(" "),
            1, 2, 0, 1);
    button_add = gtk_button_new_with_label(" Add "); 
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            button_add ,
            1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new(" "),
            1, 2, 2, 3);
    button_del = gtk_button_new_with_label("Delete");
    gtk_table_attach_defaults(GTK_TABLE(table),
            button_del,
            1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table),
            label = gtk_label_new(" "),
            1, 2, 5, 6);
    g_signal_connect (button_del, "clicked",
                    G_CALLBACK (delete_station),
                    NULL);
    g_signal_connect (button_add, "clicked",
                    G_CALLBACK (weather_window_add_station),
                    NULL);


    /* Main interface setting page */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
            table = gtk_table_new(1, 3, FALSE),
            label = gtk_label_new("Main Interface"));
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Icon size:"),
            0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
            1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),icon_size = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), "Large");
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), "Medium");
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), "Small");
    if (strcmp(_weather_icon_size,"Large") == 0)
      gtk_combo_box_set_active (GTK_COMBO_BOX(icon_size),0);
    else
      if (strcmp(_weather_icon_size,"Medium") == 0)  
       gtk_combo_box_set_active (GTK_COMBO_BOX(icon_size),1);      
      else
       gtk_combo_box_set_active (GTK_COMBO_BOX(icon_size),2);      
       
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Font color:"),
            0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
             1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label),font_color = gtk_color_button_new());
    gtk_color_button_set_color(GTK_COLOR_BUTTON(font_color), &_weather_font_color);      

    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Transparency"),
            0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
             1, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(label),chk_transparency = gtk_check_button_new ());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_transparency),
            _enable_transparency);
    /* Update page */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
            table = gtk_table_new(1, 2, FALSE),
            label = gtk_label_new("Update"));
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Update time:"),
            0, 1, 0, 1);	    
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
            1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),update_time = gtk_combo_box_new_text());



    /* Fill update time box */
    time_update_list_temp=time_update_list;
    while (time_update_list_temp != NULL)
    {
     tu = time_update_list_temp->data;
     gtk_combo_box_append_text(GTK_COMBO_BOX(update_time), tu->name_between_time);
     if  (tu->between_time == _weather_periodic_update)
      gtk_combo_box_set_active (GTK_COMBO_BOX(update_time),index_update_time);
     time_update_list_temp = g_slist_next(time_update_list_temp);
     index_update_time++;
    }

   gtk_widget_show_all(window_config);
   while(GTK_RESPONSE_ACCEPT == gtk_dialog_run(GTK_DIALOG(window_config))) /* Press Button Ok */
   {
     if (_weather_station_name) /* If not select station not exit from prefernce window */
     {
	 if ( strcmp(_weather_icon_size,gtk_combo_box_get_active_text(GTK_COMBO_BOX(icon_size))) != 0)
	 {  
	  flag_update_icon = TRUE;
	  _weather_icon_size = gtk_combo_box_get_active_text(GTK_COMBO_BOX(icon_size));	
	 }
	 gtk_color_button_get_color(GTK_COLOR_BUTTON(font_color), &_weather_font_color_temp);
	 if (( _weather_font_color_temp.red != _weather_font_color.red ) &&
	     ( _weather_font_color_temp.green != _weather_font_color.green ) &&
	     ( _weather_font_color_temp.blue != _weather_font_color.blue ))
	  {
           flag_update_icon = TRUE;     
	   _weather_font_color = _weather_font_color_temp;
	  } 
	
	 /* Trnsparency mode */
         if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_transparency)) != _enable_transparency)
         {
	  _enable_transparency = gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(chk_transparency));
           flag_update_icon = TRUE;
         } 

         /* Find select element of update time box and save time value */
         time_update_list_temp=time_update_list;
         while (time_update_list_temp != NULL)
         {
          tu = time_update_list_temp->data;
	  if (strcmp(tu->name_between_time,gtk_combo_box_get_active_text(GTK_COMBO_BOX(update_time))) == 0)
	  {
	   _weather_periodic_update=tu->between_time;
	   break;
	  }    	  
          time_update_list_temp = g_slist_next(time_update_list_temp);
         }
	    	
         config_save();

	 if (flag_update_icon)
	 {
          weather_frame_update();
	  gtk_widget_destroy(weather_window_popup);
         }
	 
	 if (flag_update_station)
	 {
	  update_weather();
          weather_frame_update();	  
         }

         gtk_widget_destroy(window_config);
	 free_list_stations();

         not_event = FALSE;
	 return;
     }
    }
  
   /* Pressed CANCEL */
   if (flag_update_station)
   {
     gtk_widget_destroy(window_config);

     if ( g_slist_length(stations_view_list) >0 )     
      update_weather();
     else
      gtk_widget_destroy(weather_window_popup);
      
     weather_buttons_init();
     weather_frame_update();
     config_save();
   }
   else
   { 
     gtk_widget_destroy(window_config);
   }
  not_event = FALSE;
}
