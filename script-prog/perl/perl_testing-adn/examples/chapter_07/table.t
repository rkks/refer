#!perl

use strict;

use Test::More tests => 1;
use Test::HTML::Lint;

html_ok( <<'EOF' );

<h1>My Favorite Sciuridae</h1>

<table>
    <trh>
        <td>Grey squirrel</td>
        <td>plump, calm</td>
    </tr>
    <tr>
        <td>Red squirrel</td>
        <td>quick, shifty</td>
    <tr>
        <td>Yellow-bellied Marmot</td>
        <td>aloof</td>
    </tr>
</table>

EOF
