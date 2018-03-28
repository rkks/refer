package InsertWrapper;
  
use strict;
use warnings;
  
use DBI;
  
sub new
{
    my ($class, %args) = @_;
    my $dbh            = DBI->connect(
        @args{qw( dsn user password )},
        { RaiseError => 1, PrintError => 0 }
    );
  
    my $self = bless { dbh => $dbh, logfh => $args{logfh} }, $class;
    $self->log( 'CONNECT', dsn => $args{dsn} );
    return $self;
}
  
sub dbh
{
    my $self = shift;
    return $self->{dbh};
}
  
sub log
{
    my ($self, $type, %args) = @_;
    my $logfh                = $self->{logfh};
  
    printf {$logfh} "[%s] %s\n", scalar( localtime(  ) ), $type;
  
    while (my ($column, $value) = each %args)
    {
        printf {$logfh} "\t%s => %s\n", $column, $value;
    }
}
  
sub insert
{
    my ($self, $table, %args) = @_;
    my $dbh                   = $self->dbh(  );
    my $columns               = join(', ', keys %args);
    my $placeholders          = join(', ', ('?') x values %args);
    my $sth                   = $dbh->prepare(
        "INSERT INTO $table ($columns) VALUES ($placeholders)"
    );
  
    $sth->execute( values %args );
    $self->log( INSERT => %args );
}
  
1;
