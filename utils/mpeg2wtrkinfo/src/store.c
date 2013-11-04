#include "config.h"
#include "global.h"

#include <stdlib.h>
#include <stdio.h>

int store_init (void)
{
  int rc;
  xml_writer = xmlNewTextWriterFilename(Output_Wtrkinfo_Filename, 0);
  if (xml_writer == NULL) 
    return 1;
  rc = xmlTextWriterStartDocument(xml_writer, NULL, "ISO-8859-1", NULL);
  if(rc < 0)
    goto error;

  rc = xmlTextWriterStartElement(xml_writer, BAD_CAST "ENTRIES");
  if(rc < 0)
    goto error;

  return;

error:
    return 1;
}

void store_end (void)
{
  int rc;
  rc = xmlTextWriterEndElement(xml_writer);
  if (rc >= 0) 
    rc = xmlTextWriterEndDocument(xml_writer);
}

void store_entry (char **values, int size, int position, char *default_value)
{
  int rc, i;
  char buffer[DEFAULT_BUFFER_SIZE];

  rc = xmlTextWriterStartElement(xml_writer, BAD_CAST "ENTRY");
  if(rc < 0)
    goto error;

  snprintf(buffer, DEFAULT_BUFFER_SIZE, "%d", position);

  rc = xmlTextWriterWriteAttribute(xml_writer, BAD_CAST "position", BAD_CAST buffer);
  if(rc < 0)
    goto error;


  for(i=0; i<size; i++)
  {
    rc = xmlTextWriterWriteFormatElement(xml_writer, BAD_CAST "v", "%s", values[i]);
    if (rc < 0)
      goto error;
  }

  rc = xmlTextWriterWriteFormatElement(xml_writer, BAD_CAST "default_value", "%s", default_value);
  if (rc < 0)
    goto error;


  rc = xmlTextWriterEndElement(xml_writer);
  if(rc < 0)
    goto error;

  return;
 error:
  fprintf(stderr, "store_entry: unexpected error");
  exit(1);
}