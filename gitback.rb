require "yaml"
require "open-uri"

username = "addyosmani"

time = Time.new
backupDirectory = "/backups/github/#{time.year}.#{time.month}.#{time.day}"


YAML.load(open("http://github.com/api/v2/yaml/repos/show/#{username}"))
['repositories'].map{|repository|
	puts "discovered repository: #{repository[:name]} ... backing up ..."
	system "git clone susienazaryan/os161@github.com:#{username}/#{repository[:name]}.git #{backupDirectory}/#{repository[:name]}"
}

