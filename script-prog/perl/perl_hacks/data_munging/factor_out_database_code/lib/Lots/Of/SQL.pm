package Lots::Of::SQL;

use base 'Exporter';
use vars qw( @EXPORT_OK %EXPORT_TAGS );

@EXPORT_OK = qw(
    select_user    insert_user    update_user  
    select_story   insert_story   update_story 
    select_comment insert_comment
    select_stories
    select_user_stories
    select_user_comments
);

%EXPORT_TAGS = (
    user    =>
    [ qw(
        select_user insert_user update_user select_user_stories 
        select_user_comments
    )],
    story   =>
    [ qw(
        select_story insert_story update_story select_user_stories
        select_stories
    )],
    comment => [ qw( select_comment insert_comment select_user_comments )],
);

sub select_nodemethod_attributes ()
{
    return <<'END_SQL';
    SELECT
        types.title     AS class,
        methods.title   AS method,
        nodemethod.code AS code
    FROM
        nodemethod
    LEFT JOIN
        node AS types ON types.node_id = nodemethod.supports_nodetype
END_SQL
}

1;
