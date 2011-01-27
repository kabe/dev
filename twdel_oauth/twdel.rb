require 'rubygems'
require 'oauth'
require 'rubytter'
require 'pit'

config = Pit.get("twdel", :require => {
                   "consumerkey" => "Client's consumer key",
                   "consumersecret" => "Clients' consumer secret",
                   "accesstoken" => "Input your Access Token",
                   "accesssecret" => "Input your Access secret",
                   "username" => "input your user name",
                 })

consumer = OAuth::Consumer.new(
                               config["consumerkey"],
                               config["consumersecret"],
                               :site => 'http://twitter.com'
                               )

token = OAuth::AccessToken.new(
                               consumer,
                               config["accesstoken"],
                               config["accesssecret"]
                               )

rubytter = OAuthRubytter.new(token)

p rubytter.user(config["username"])

print "Limit status: "
p rubytter.limit_status()
rubytter.user_timeline(config["username"], :page => 5).each do |status|
    puts "Delete status: #{status.text}"
    rubytter.remove_status(status.id)
end

