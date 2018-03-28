use Mac::Growl;

Mac::Growl::RegisterNotifications(
    'growlalert', # application name
    [ 'alert' ],  # notifications this app sends
    [ 'alert' ],  # enable these notifications
);
