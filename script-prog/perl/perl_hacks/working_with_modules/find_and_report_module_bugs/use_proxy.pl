# Create a proxy object
my $proxy = FooProxy->new();

# Make sure the proxy acts like a Foo
if($proxy->isa('Foo'))
{ 
  print "Proxy is a Foo!\n";
}
else
{ 
  die "Proxy isn't a Foo!";
}
