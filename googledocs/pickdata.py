#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import cgi
import csv

from pit import Pit
import gdata.service
import gdata.docs
import gdata.docs.service
import gdata.spreadsheet.service


def swap_logintoken(client, new_token):
    """
    """
    client.SetClientLoginToken(new_token)


def main():
    """
    """
    credentials = get_credentials()
    gd_client = gdata.docs.service.DocsService(source='hoge-v1')
    gd_client.ClientLogin(credentials["login"], credentials["passwd"])
    spreadsheet_client = gdata.spreadsheet.service.SpreadsheetsService()
    spreadsheet_client.ClientLogin(credentials["login"], credentials["passwd"])
    tokens = {"docs": gd_client.GetClientLoginToken(),
              "spreadsheet": spreadsheet_client.GetClientLoginToken()}
    #q = gdata.docs.service.DocumentQuery(categories=['spreadsheet'])
    q = gdata.docs.service.DocumentQuery()
    q["title"] = "連絡先集め"
    q["title-exact"] = "true"
    q2 = gdata.docs.service.DocumentQuery()
    q2["title"] = "合宿参加者連絡先"
    q2["title-exact"] = "true"
    #feed = gd_client.GetDocumentListFeed()
    feed = gd_client.Query(q.ToUri())
    #PrintFeed(feed)
    assert(len(feed.entry) == 1)
    entry = feed.entry[0]
    feed2 = gd_client.Query(q2.ToUri())
    new_entry = feed2.entry[0]
    file_path = 'connections.csv'
    new_file_path = 'connections.new.csv'
    print 'Downloading spreadsheet to %s...' % (file_path,)
    swap_logintoken(gd_client, tokens["spreadsheet"])
    gd_client.Export(entry, file_path, gid=1)
    swap_logintoken(gd_client, tokens["docs"])
    edit_csv(file_path, new_file_path)
    ms = gdata.MediaSource(
        file_path=new_file_path,
        content_type=gdata.docs.service.SUPPORTED_FILETYPES['CSV'])
    new_entry.title.text = q2["title"]
    upd8ed_entry = gd_client.Put(ms, new_entry.GetEditMediaLink().href)
    print 'Spreadsheet now accessible online at:', upd8ed_entry.GetAlternateLink().href


def edit_csv(readpath, writepath):
    """Edit CSV file.
    
    @param path
    """
    assert(readpath != writepath)
    fields = []
    with open(readpath) as f:
        reader = csv.DictReader(f, delimiter=",")
        fields = reader.fieldnames[1:6]
        for field in fields:
            print field
        with open(writepath, "w") as g:
            writer = csv.DictWriter(g, fieldnames=fields, delimiter=",")
            writer.writerow(dict((k, k) for k in fields))
            for line in reader:
                row = dict((k, line[k]) for k in fields)
                writer.writerow(row)


def PrintFeed(feed):
    """Prints out the contents of a feed to the console."""
    print '\n'
    if not feed.entry:
        print 'No entries in feed.\n'
    for entry in feed.entry:
        print '%s %s %s' % (
            entry.title.text,
            entry.GetDocumentType(),
            entry.resourceId.text)


def get_credentials():
    """Get credential information from Pit.
    """
    conf = Pit.get("Google")
    if not conf:
        # If not exists, exception will be raised
        editor = os.environ["EDITOR"]
        conf = Pit.get(
        'Google', {
                'require':{'login':'Google ID','passwd':'Google PassWord'}})
    return conf

if __name__ == '__main__':
    #print 'Login to your Google account by going %s' % GetAuthSubUrl()
    main()
