package Users;
  
use strict;
use warnings;
  
my $dbh;
  
sub set_db
{
    my ($self, $connection) = @_;
    $dbh                    = $connection;
}
  
sub fetch
{
    my ($self, $column, $value) = @_;
  
    my $sth = $dbh->prepare( 
        "SELECT id, name, age FROM users WHERE $column = ?" );
  
    $sth->execute( $value );
  
    return unless my ($id, $name, $age) = $sth->fetchrow_array(  );
    bless { id => $id, name => $name, age => $age, _db => $self }, 'User';
}
  
sub create
{
    my ($self, %attributes) = @_;
    my $sth                 = $dbh->prepare(
        'INSERT INTO users (name, age) VALUES (?, ?)'
    );
  
    $sth->execute( @attributes{qw( name age )} );
    $attributes{id} = $dbh->last_insert_id( undef, undef, 'users', 'id' );
    bless \%attributes, 'User';
}
  
package User;
  
our $AUTOLOAD;
  
sub AUTOLOAD
{
    my $self     = shift;
    my ($member) = $AUTOLOAD =~ /::(\w+)\z/;
    return $self->{$member} if exists $self->{$member};
}
  
1;
