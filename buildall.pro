TEMPLATE = subdirs

# build as ordered or build will fail
CONFIG += ordered

SUBDIRS = fplib lastfmfpclient

unix:system( cp lastfmfpclient/lastfmfpclient . )
