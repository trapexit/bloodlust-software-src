
#define DONE   -1
#define DELAY  0
#define ICER   1
#define ETHAN  2
#define STUMP 3

//struct for text
typedef struct nogtext 
{
 int person;
 int delay;
 char *text;
} nogtext;



//test conversation
nogtext intro[]= {
 {DELAY,1100,0},     
 {ICER ,400, "Wow! Moving paddle action."},
 {DELAY,200,0}, 
 {ETHAN,300, "Indeed. I could play this all day."}, 
 {STUMP,0, "Well, you two better get used to it!"}, 
 {STUMP,0, "I'm going to keep you strapped here in total agony until you reveal to me the "
        "secret location of Bloodlust Software World Headquarters!!"},
 {DELAY,200,0}, 
 {ICER ,0, "Blow it out of your undersized ass, Stumpy."},
 {STUMP,0, "Ahh..So I see that the heightling wants to play!"},
 {STUMP,0, "We shall play, oh yes, we shall play..."},
 {DELAY,600,0}, 
 {DONE,0,0}
 };


nogtext shutup[]= {
 {STUMP,0, "I shall keep you two infidels trapped in the room of eternal pong forever!"},
 {ETHAN,0, "Someone needs to tell that midget to shut up...."}, 
 {ETHAN,0, "..or else I'll pound him into half a midget."},
 {ICER ,0, "....isn't that like an eighth of a real man?"}, 
 {ETHAN,0, "..or a sixteenth of Andre the Giant."},
 {DELAY,200,0}, 
 {ICER ,0, "Midgets serve only one purpose, and that's chucking."}, 
 {ETHAN,0, "The only good midget is a chucked midget, I always say."},
 {DELAY,200,0}, 
 {DONE,0,0}
 };


//test conversation
nogtext squat[]= {
 {ICER ,400, "Hey, do you know what the term 'squat launching' means exactly?"},
 {DELAY,200,0},
 {ETHAN,0, "Are you a retard or something?"},  
 {ETHAN,0, "You DON'T know what the term 'squat launching' means?"},
 {ICER ,0, "Wait a second, buddy..."},
 {ICER ,0, "Are you trying to tell me that everyone who's not a retard knows what a 'squat launch' is?"},
 {ETHAN,0, "Precisely."},
 {ICER ,0, "Look here, if everyone who didn't know what 'squat launching' means is a tard, then there "
            "wouldn't be enough tardwagons in existence to take everyone bowling."},
 {ICER,0, "It's just mathematically impossible."},             
 {ICER,0, "Hmmm, maybe you could transport the tards using a long bus, instead of the short ones."},             
 {ICER,0, "You could transport twice the tards in half the time."},             
 {ETHAN,0, "Are YOU INSANE or something??"},
 {ETHAN,0, "You can't use the long bus to transport tards! Its a violation of all the tardwagon driver ethics "
           "that I abide by!"},
 {ETHAN,0, "You'll disrupt the whole space-tard continuum!"},
 {ICER,0, "Oh, dear me! What was I thinking!?"}, 
 {DELAY,200,0}, 
 {DONE,0,0}
}; 
 
 
//test conversation
nogtext notliketard[]= {
 {ICER ,400, "Hey, I don't think you like tards as much as I do."},
 {ETHAN,0, "Listen, as a tardwagon driver myself, I happen to know tards like no one else knows tards."},
 {ETHAN,0, "And let me tell you something, buddy..."},
 {ETHAN,0, "If you don't have your tard card, you aint getting on the tardwagon."},
 {ICER ,0, "What if I had a letter from my mom?"},
 {ETHAN,0, "Nope, sorry buddy."}, 
 {ICER ,0, "What if I provided you with drool samples?"},
 {ETHAN,0, "Nope, no can do, buddy."},
 {ICER ,0, "What if I wore a black cardboard box and did some Downy performance art?"},
 {DELAY,200,0},  
 {ETHAN,0, "If you don't have your tard card, you aint getting on the tardwagon."},
 {ETHAN,0, "It's the rules of the road, buddy."},
 {DELAY,200,0}, 
 {DONE,0,0}

};


nogtext buddy[]={
 {ICER ,0, "Hey buddy, stop calling me buddy!"},
 {ETHAN,0, "Sorry, bad habit. I'll try to stop, Ramona."},
 {ICER ,0, "Hey, my name's not Ramona..."},
 {ETHAN,0, "Sorry about that, Ramona."}, 
 {DELAY,200,0},
 {ETHAN,0, "Ramona...would you be a dear and pass me some of those crumpets?"},
 {ICER ,0, "Call me Ramona again and I'll be forced to bludgeon you."},
 {DELAY,200,0}, 
 {DONE,0,0}
};

//test conversation
nogtext tirebiter[]= {
 {ICER ,600, "..Shweee bop. Shweee bop bop da ta da..."},
 {ETHAN,0, "..shwee bop da ta da..bop da ta da ta..."},
 {DELAY,400,0},
 {ICER ,0, "Nothing like some tirebiter tunes to get your toes a' tapping."},
 
 {DELAY,800,0}, 
 {DONE,0,0}
};

//test conversation
nogtext piss[]= {
 {ETHAN,0, "Damn. I'll be right back. I gotta go take a pee pee."},
 {ICER ,0, "Don't go scoop diving..."},
 {ETHAN,0, "Damn! ARGGH...Stumpy really bolted us down good!"},
 {ETHAN,0, "*grunt* I can't get out of my electric chair! *grunt*"},
 {ICER ,0, "Looks like the floor will have a new yellow shine pretty soon..."},
 {ETHAN,0, "Stumpy!! You bastard! How do you expect us to go tinkle when we are strapped down like this?"},
 {STUMP,0, "You pathetic tall man! Even if I were to free you, there's no way you could use the restroom here."},
 {STUMP,0, "All of the urinals have been lowered to midget-heights, and are unusable by heightlings such as yourself!"},
 {ETHAN,0, "DAMN YOU!! How clever of you to lower those urinals..."},
 {ETHAN,0, "I am defeated."},
 {DELAY,400,0}, 
 {DONE,0,0}
};

nogtext nipple[]={
 {ETHAN,0, "I'm starting to like this electricity, it's making my nipples perk up."},
 {ICER,0,  "Yep."},
 {ICER,0,  "Perks them up quicker than a pencil flicking."}, 

 {DELAY,400,0}, 
 {DONE,0,0}
} ;
 

nogtext bud[]={
 {ICER,100,  "Time."},
 {DELAY,50,0}, 
 {ETHAN,100,  "Er."},
 {DELAY,50,0},
 {STUMP,100,  "Slaught."},
 {DELAY,50,0},
 {ETHAN,100,  "Er."},
 {DELAY,50,0},
 {ICER,100,  "Time."},
 {DELAY,50,0}, 
 {STUMP,100,  "Slaught."},
 {ICER,80,  "Time."},
 {ETHAN,80,  "Er."},
 {STUMP,80,  "Slaught."},
 {DELAY,100,0},
 {ETHAN,80,  "Er."},
 {ICER,80,  "Time."},
 {DELAY,100,0}, 
 {ICER,80,  "Time."},
 {STUMP,80,  "Slaught."},
 {ETHAN,80,  "Er."},
 {DELAY,400,0},
 {DONE,0,0}
} ;

nogtext bulge[]={
 {ICER,0,  "Hey, Stumpy...what's that unusual bulge in the front of your tights?"},
 {ETHAN,0, "EWWW...what kind of question is that?"},
 {ICER,0,  "It's just a question."},
 {ETHAN,0, "You midget-crotch-looker!"},
 {ICER,0,  "I wasn't looking at his crotch!"},
 {ICER,0,  "It's very rare that I'm eye-level with a midget's crotch."},
 {ETHAN,0, "Midget-crotch-looker!"}, 
 {STUMP,0,  "Enough bickering about my package!"},
 {ICER,0,  "It seems like Stumpy had some limb-lengthening surgery..."},
  
 {DELAY,400,0}, 
 {DONE,0,0}
} ;


nogtext underwear[]={
 {ETHAN,0, "Okay, real quick....underwear check!!"},
 {ICER,0,  "I'm wearing boxers."},
 {ETHAN,0, "I'm wearing boxers here too...."},
 {DELAY,100,0}, 
 {STUMP,0,  "I'm wearing...ummm...errrr...."},
 {ICER,0,  "Cough it up Stumpo! It's an underwear check..."},
 {STUMP,0, "...wait a second! This is some midget joke isn't it?"},
 {ETHAN,0, "No it's not."},
 {ETHAN,0, "It's just an underwear check."},
 {STUMP,0, "You want me to say that I wear briefs, don't you?!"},
 {STUMP,0, "Briefs! I wear briefs! Are you guys happy now??!"},
 {ICER,0, "Hehehe."},
 {ETHAN,0, "Hehehe."},  
   
 {DELAY,400,0}, 
 {DONE,0,0}
} ;

    
nogtext cavity[]={
 {ETHAN,0, "Hey, Stumpy, do you have any cavities?"},
 {STUMP,0,  "No."},
 {DELAY,300,  0},
 {ETHAN,200, "Heheheh..."},
 {ICER,0,  "Hehehehe!"},
 {ETHAN,200, "Hahahaahaa..."},
 {ICER,200,  "Tee hee tee hee!!"},
 {ETHAN,200, "Ho ho ho ho ho ho."},
 {ICER,0,  "Why Stumpy, I didn't know you were a dog's best friend."},

 {DELAY,400,0}, 
 {DONE,0,0}
} ;

//test conversation
nogtext president[]= {
 {ICER ,600, "Who are you gonna vote for in the Presidential Election of 96?"},
 {ETHAN,0, "Presidential erection?"},
 {ICER ,0, "I always bet on black."},
 {STUMP,0, "Hey, I'm running for president in '96."},
 {STUMP,0, "Midget brutality will end when I'm in office."},
 {STUMP,0, "I'm going to replace all the drunk congressmen with more capable drunk midgets."},
 {STUMP,0, "One nation under Stumpy, with foldable stools and ministilts for all!"},
 {DELAY,300,0},
 {DONE,0,0}
};

nogtext luger[]= {
 {ETHAN ,600, "Hey man, what's up with you and Luger?"},
 {ICER ,0, "Lemme tell ya something brother..."},
 {ICER ,0, "..don't be talking that way unless you want a boot sandwich."},
 {ETHAN ,0, "Oh no! Not that!"},
 {ICER,0, "...and after you've had your sandwich, I'm going to give you leg drop soup for dessert."},
 {STUMP,0, "It's a total debauchle!"},
 {DELAY,300,0},
 {DONE,0,0}
};

nogtext easychair[]={
 {ETHAN,0, "Man, these electric chairs are nice and comfy."},
 {ICER,0,  "You know ALL about easy chairs."},
 {ETHAN,0,  "I know ALL about easy chairs."}, 
 {DELAY,400,0}, 
 {DONE,0,0}
} ;

nogtext captainplanet[]={
 {ICER,0,  "Okay, explain this to me."},
 {ICER,0,  "You've got your four basic powers...."},
 {ICER,0,  "..you've got your fire, you got your water..."},
 {ICER,0,  "..there's earth and then you've got your wind."},
 {ICER,0,  "Now. Could some one please explain to me what heart does?"},
 {ICER,0,  "Each of 'em has all these cool powers of the elements..."},
 {ICER,0,  "and then there's a little turd with a 'heart' ring!?"},
 {ICER,0,  "What the HELL does heart do? It's worthless!"},
 {ETHAN,0,  "Man, you know nothing of the Planeteers, do you?"},
 {ETHAN,0,  "You need all 5 rings to summon Captain Planet..."},
 {ETHAN,0,  "It's common knowledge."},
 {ICER,0,  "I still think heart is useless."},
 {STUMP,0,  "'Looting and polluting is the thing to do...'"},
 {DELAY,400,0}, 
 {DONE,0,0}
};

nogtext bulldozers[]={
 {ICER,0,  "Bulldozers are cool. "},
 {ETHAN,0, "Yeah."},
 {ICER,0,  "Hard Hat Harry(TM) is a living god."},
 {ETHAN,0, "Yeah."},
 {ICER,0,  "I could spend all day watching cows."},
 {ETHAN,0, "Yeah."}, 
 {ICER,0,  "Those monotremes really know how to keep you coming back for more."},
 {ETHAN,0, "They sure are a work of evolutionary genius."},
 {ETHAN,0, "Two holes for the price of one!"},
 {DELAY,400,0}, 
 {DONE,0,0}
};

nogtext facesofdeath[]={
 {ICER,0,  "Ever wanted to know what the inside of a leg looked like?"},
 {ETHAN,0, "NOW you know!"},
 {ETHAN,0,  "I like the part where they blowtorch the live pigs..."},
 {ICER,0,  "Have you seen Faces of Death 3? "},
 {ETHAN,0, "Not only have I seen Faces of Death part 3..."},
 {ETHAN,200, "I've seen Traces of Death 1, 2, and 3..."},
 {ETHAN,200, "...I've seen Shadows of Death 1 and 2..."},
 {ETHAN,200, "...umm, also Death Emporium, Smorgasboard of Death, Museum of Death..."},
 {ETHAN,200, "I saw the collector's edition of Dead Death..."},
 {ETHAN,  0, "..and also Pot of Death."},
 {DELAY,100,0},
 {ICER,0,  "Death Pot comes from the sky."},
 {ETHAN,0,  "It does!"},
 {DELAY,400,0},
 {DONE,0,0}
};


nogtext jihad[]={
 {ICER,0,  "Jihad! "},
 {ICER,0,  "Jihad! Jihad! JIHAD!!"},
 {ETHAN,0, "You can't wage a Jihad."},
 {ICER,0,  "Why not?"},
 {ETHAN,0, "Because you aren't wearing your hawaiian shirt."},
 {ICER,0,  "I will be pinging the pong."},
 {DELAY,400,0},
 {DONE,0,0}
};

nogtext scooby[]={
 {ICER,0,  "Hey look! It's a squeeky board on the floor."},
 {ICER,0,  "...I think I'll fondle it with my foot."},
 {ETHAN,0, "And look! There are midget footprints all over the ceiling!."},
 {ICER,0,  "Something's smells midgety about all this..."},
 {ETHAN,0, "Wait! I've got the mystery solved!"},
 {ETHAN,0,  "Stumpy used to be a carnival acrobat, and he doesn't need suction cups to climb on ceilings!"},
 {ETHAN,0, "He must have done it!"},
 {STUMP,0, "You're right."},
 {STUMP,0, "And I would have gotten away with it if it weren't for you meddling perverts and that horny dog."},
 {DELAY,400,0},
 {DONE,0,0}
};


nogtext shadow[]={
 {ICER,0,  "brdbrbdbrbbdrbrbdbrbrd.....UGH! "},
 {ETHAN,0, "Good Shadow impersonation."},
 {ICER,0,  "I like Carlos."},
 {ETHAN,0, "You mean Erica?"},
 {ICER,0,  "What are you talking about? They look nothing alike, Carlos is a man."},
 {ETHAN,0, "Maybe so..."}, 
 {ICER,0,  "My favorite Erica Storm quote is..."},
 {ICER,0,  "'You're just like all the street trash I wasted before!'"},
 {ETHAN,0, "Erica didn't say that, Carlos did."}, 
 {ETHAN,0, "'Is that all you got?'"}, 
 {ETHAN,0, "'Is that ALL you got?'"}, 
 {DELAY,400,0},
 {DONE,0,0}
};


nogtext beer[]={
 {STUMP,0,  "Now you two alcohol swilling swine will fall before me."},
 {ETHAN,0, "I like drinking beer out of the tall cans."},
 {STUMP,0,  "Don't mock me."},
 {ICER,0, "I dunno, the long neck bottles are pretty good."},
 {STUMP,0,  "Shut up."},
 {ETHAN,0, "Now that Australian beer, that is in really large cans."}, 
 {STUMP,0,  "Nevermind..."},
 {DELAY,400,0}, 
 {DONE,0,0}
};


nogtext strode[]={
 {ETHAN,0, "dum dum dum dum...dum dum...STRODE!"},
 {DELAY,400,0}, 
 {DONE,0,0}
};


nogtext car[]={
 {ICER,0,  "Wouldn't that suck if someone took a dump on your car window? "},
 {ETHAN,0, "With me in it? I would kick his ass."},
 {ICER,0,  "No, its a really big guy and you are strapped in."},
 {ETHAN,0, "I'd drive away."},
 {ICER,0,  "No, what if the car won't start?"},
 {ETHAN,0, "That would suck."}, 
 {ICER,0,  "He squats and craps ever so slowly on your windshield as you sit there helpless."},
 {ETHAN,0, "Stop it!"},
 {ICER,0,  "You struggle to block the flow by turning on your wipers, but alas, they are no match."},
 {ETHAN,0, "Mommmmmmmmmmy!"},
 {ICER,0,  "Yeah, that's what I thought."},
 {STUMP,0, "How can you people be so damned jovial while I am electrocuting you?"},

 {DELAY,400,0},
 {DONE,0,0}
};


nogtext wow[]={
 {ETHAN,0, "Wow, someone's still playing our game."},
 {ICER,0,  "Stumpy, did you strap them in too?"},
 {STUMP,0, "Nope, not me..."},
 {ICER,0,  "Maybe they actually like this game."},
 {ETHAN,0, "No way, they must not have a life."}, 
 {ICER,0,  "They should be playing TIMESLAUGHTER, the one on one fighting beauty by Bloodlust Software."},
 {ETHAN,0, "Due out summer 1993!"},
 {ICER,0,  "But wait, it's 1996."},
 {ETHAN,0, "I guess the release date got moved back a tad."},
 {ICER,0,  "You call three years a tad?"},
 {ETHAN,0, "ok, maybe more like a smidgeon then."},
 {DELAY,400,0},
 {DONE,0,0}
};
 

nogtext homepage[]={
 {ETHAN,0, "Hey person playing....go to our homepage!"},
 {ICER,0,  "Yeah, you can get all kinds of sick pictures, anti-morality propaganda, and games."},
 {STUMP,0, "Stay away from this page...they only want to make you hate midgets!"},
 {ICER,0,  "Do a net search for Bloodlust or midget, you'll find it!"},
 {ETHAN,0, "I heard that page is really tasteless."}, 
 {ICER,0,  "Yeah, I love it."},
 {DELAY,400,0},
 {DONE,0,0}
};


nogtext cable[]={
 {ETHAN,0, "Hey, did you remember to pay your cable bill?"},
 {ICER,0,  "Ummm......"},
 {ETHAN,0, "You better pay it or else Sergeant Toochie will be after you."}, 
 {ICER,0,  "I think I'll just bribe him, that's the way the system works for Toochie."},
 {ETHAN,0, "We also better hide the stash of white slave girls in our basement."}, 
 {ICER,0,  "Yes! We must! I'll get Ralphus right on it..."}, 
 {ETHAN,0, "She will dance, oh yes she will dance!"}, 
 {ICER, 0, "She will dance for our show!"}, 
 {DELAY,400,0},
 {DONE,0,0}
};

nogtext rimmer[]={
 {ICER,0,  "Hey, if you had a daughter, what would you name her?"},
 {ETHAN,0, "I think that I'd name her 'Rimmer'."}, 
 {ICER,0,  "Oh yeah?"},
 {ETHAN,0, "Yeah, that way, if she has to escape from a space station..."}, 
 {ETHAN,0, "...someone might say to her: "}, 
 {ETHAN,0, "'It's time to pull out, Rimmer.'"}, 

 {DELAY,400,0},
 {DONE,0,0}
};

nogtext demon[]={
 {ICER,0,  "That would suck to be chased by the lord of hell, if he happened to arrive on Earth one day."},
 {ETHAN,0, "Well, not really..."}, 
 {ETHAN,0, "..you just gotta know how to deal with them there demons..."}, 
 {ICER,0,  "Oh yeah?"},
 {ETHAN,0, "Yeah!"}, 
 {ETHAN,0, "If he forces you to walk down a hallway..."}, 
 {ETHAN,0, "..all you gotta do is run through a door and shut it behind you!"}, 
 {ETHAN,0, "He'll never be expecting it."}, 
 {ETHAN,0, "Those lords of pain don't know how to operate modern doorknobs."}, 
 {ICER,0,  "Boy, you sure know a lot about battling demons and stuff."},
 {DELAY,400 ,0},
 {DONE,0,0}
};

nogtext retardation[]={
 {ICER,0,  "We don't need no retardation...."},
 {ETHAN,0, "We don't need no limb control."}, 
 {STUMP,0, "Teacher! Leave those tards alone!"}, 
 {ICER,0,  "All in all, its just another tard drooling on the wall."},
 {DELAY,400,0},
 {DONE,0,0}
};

nogtext peepee[]={
 {ETHAN,0, "When I get rich and famous, I'm going to hire a maid."}, 
 {ICER,0,  "Oh yeah? Why is that?"},
 {ETHAN,0, "That way, when she cleans my bathroom she might touch my pee pee."}, 
 {ICER,0,  "I can see how that would be nice."},
 {ETHAN,0, "And..you know, if I'm lucky she may touch my poo poo also."}, 
 {ICER,0,  "Such dreams and aspirations you have...."},
 {DELAY,400,0},
 {DONE,0,0}
};


nogtext chili[]={
 {STUMP,0, "You look like someone who would like some all-you-can-eat chili..."}, 
 {ICER,0,  "No thanks."},
 {STUMP,0, "You look like a growing chili boy!"}, 
 {ICER,0,  "No really, I'm stuffed."},
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext executioners[]={
 {STUMP,0, "Hey, you guys look really familiar...."},
 {ICER,0,  "We do?"},
 {STUMP,0, "Yeah."}, 
 {STUMP,0, "Weren't you the guys that made Executioners??"}, 
 {ETHAN,0, "Executioners? Ummm...errr.... uhhhhh..."},
 {ICER,0,  "Well, gee...uhhh, well..hmmmm..."},
 {ETHAN,0, "You mean that one side-scrolling fighting game with huge characters, tons of personality, and midget-chucking action?"},
 {STUMP,0, "Yeah!! That's it! 'Executioners'..."},
 {ETHAN,0, "Nope, never heard of it."},
 {ICER,0,  "Executioners? Nope, draws a blank..."},
 
 {DELAY,600,0},
 {DONE,0,0}
};


nogtext grind[]={
 {STUMP,0, "After we get done in here...you guys are REALLY going to suffer!"},
 {ICER,0,  "I'm really scared."},
 {STUMP,0, "I'm going to take you two into the room of eternal pain and torture."}, 
 {ETHAN,0, "Oh, is that the one room with the reruns of The Grind playing?"},
 {STUMP,0, "Yeah, how'd you know?"},
 {ETHAN,0, "Just a wild guess."},
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext nn3[]={
 {ICER,0,  "Hey, I wonder if we'll still be strapped to electric chairs in Noggin Knockers 3?"},
 {ETHAN,0, "I doubt it."},
 {ICER,0,  "Maybe in Noggin Knockers 3 we could actually spend more than a week making the game..."},
 {ICER,0,  "...and we could put in complex torture devices!"},
 {ETHAN,0, "Complex torture devices? Like what?"},
 {ICER,0,  "Well, we could have a sharp drill that bores deeper into our heads each time we miss..."},
 {ETHAN,0,  "Ouch! That would hurt!"},
 {ICER,0, "We could have a fat urine-smelling woman with one nipple dance erotically for the loser."},
 {ETHAN,0,  "We could hire Zoe real cheap."},
 {ICER,0,  "We could have a Craig Mack CD blasting in the ears of the loser...."},
 {ETHAN,0,  "That would be sheer torture!"},
 {ETHAN,0,  "I'd be on my knees, burning begging, baby please!"},
 {DELAY,600,0},
 {DONE,0,0}
};


nogtext stars[]=
{
 {ICER,0,  "Have you ever looked up at the stars and wondered about the vastness of the universe?"},
 {ETHAN,0, "Yeah, actually I have..."}, 
 {ICER,0,  "When you realize how big everything is, it just really makes you feel small."},
 {STUMP,0, "Shut-up."}, 
 {ETHAN,0, "Yeah, I look up at the things above me and I feel so darn tiny."}, 
 {STUMP,0, "Shut-up!"}, 
 {ICER,0,  "You don't know how much I'd like to put on super-tall ministilts so that I could touch the stars."},
 {ETHAN,0, "Yeah, all those billionszz and billionsz of starsszh."}, 
 {ETHAN,0, "You know, when a star blows up it gets turned into a white dwarf?"}, 
 {ICER,0, "Man, I sure feel sorry for that star. I bet all the tall stars pick on the white dwarfs."}, 
 {STUMP,0, "Enough of the midget puns already!"}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext tb[]={
 {ETHAN,0, "Tirebiter! Tirebiter! Tirebiter!"},
 {DELAY,400,0}, 
 {DONE,0,0}
};


nogtext goldbond[]=
{
 {ETHAN,0, "The itching...the itching and the burning...itching!!"},
 {ETHAN,0,  "The male itch...the itching..scratching....red rash... itch!"},
 {ICER,0, "Uh oh, not this again..."},
 {ETHAN,0,  "...the burning! ...the itching! ...red, puffy rash! Winter itch!"},
 {ICER,0, "Calm down! I think you're having a GoldBond (TM) flashback again!"},
 {ETHAN,0,  "Ahhhhh...that GoldBond (TM) powder helped to relieve my male itch."},
 {DELAY,400,0}, 
 {DONE,0,0}
};


nogtext astonish[]={
 {ICER,0,  "You know, I don't think this game is very astonishing."},
 {ETHAN,0, "Why not? It has humor, perversion, anti morality..."}, 
 {ICER,0,  "It doesn't have computer rendered graphics or digitized characters."},
 {STUMP,0, "Yes, you guys think art means you actually have to draw stuff."}, 
 {ETHAN,0, "So you think if I threw on some pajamas and filmed myself it would be better."}, 
 {STUMP,0, "I would buy it...you know all artists are going to be replaced by computers."}, 
 {ICER,0,  "Then they will be astonishing, we don't need any artistic style to succeed."},
 {ETHAN,0, "Yeah, polygons and live video are the new wave, I guess I'd better quit."}, 
 {ETHAN,0, "Maybe I can get a job posing as a karate guy for a new three dimensional game."}, 
 {ICER,0, "No, you have to be a scrawny little computer geek for that."}, 
 {STUMP,0, "I think you should call this game Ultimate Noggin Kombat Alpha Warriors Jet."}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext hairpresident[]={
 {ETHAN,0, "I'm going to kill the president..."},
 {ETHAN,0, "...of the Hair Club for Men."},
 {ICER,0, "Don't even joke like that."},
 {ICER,0, "You can be arrested for threatening the life of the president."},
 {ETHAN,0, "That's ridiculous, what about our first ammendment?"},
 {ICER,0, "You should know by now the government makes the laws, they don't follow them."},
 {ETHAN,0, "Oh yeah, I guess you are right."},
 {ICER,0, "Welcome to democracy."},
 {ETHAN,0, "I'm going to get him in the dining room with the lead pipe."},
 {ICER,0, "hehe."},
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext beanbag[]={
 {ETHAN,0, "I don't wanna grow up, I'm a beanbag kid!"},
 {DELAY,600,0},
 {DONE,0,0}
};


nogtext lolli[]=
{
 {ICER,0,  "Hey stop with that self stimulating behavior!"},
 {ETHAN,0, "No."}, 
 {ICER,0,  "What if I give you a lollipop?"},
 {ETHAN,0, "Ok."}, 
 {ICER,0,  "There, that's better, I'm so gla...HEY! I told you to stop!"},
 {ETHAN,0, "This way I get to have fun AND I get free lollipops, why would I stop?"},

 {DELAY,600,0},
 {DONE,0,0}
};

nogtext Review[]={
 {ICER,0,  "Hey the person playing could be a game reviewer!"},
 {ETHAN,0, "Uh Oh."}, 
 {ICER,0,  "Really, game reviewer person, this isn't the extent of our skills."},
 {ETHAN,0, "Yes, we just made this between good games for fun."}, 
 {ICER,0,  "I know! I'll bribe the game reviewer!"},
 {ETHAN,0, "Yes, use your programming skills to transfer 500 dollars to their bank account."}, 
 {ICER,0,  "Ok, transferring funds..."},
 {ETHAN,0, "There, now we get a good game review through true American methods."}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext goth[]={
 {ICER,0,  "Oooooh look at you in all black, what are you into the goth scene?"},
 {ETHAN,0, "Just because I wear fake vampire teeth and black lipstick doesn't mean I am Goth."}, 
 {ICER,0,  "What about your candle collection and the way you drink blood?"},
 {ETHAN,0, "Chicks dig guys who drink blood."}, 
 {ICER,0,  "Really, I always chew on their intestines, maybe that's what I'm doing wrong."},
 {ETHAN,0, "See, you can learn a lot from a Got...er, guy who wears black."}, 
 {STUMP,0,  "You two are disgusting, we midgets know exactly what pleases a woman."},
 {ETHAN,0, "Too bad you can't reach it."}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext lpa[]={
 {ETHAN,0, "What does that LPA on your shirt stand for, Smegma?"}, 
 {STUMP,0,  "It stands for the Little Person's Association. I'm the President."},
 {ETHAN,0, "I didn't know you supported Stumpy's cause."}, 
 {ICER,0,  "I don't, it stands for Little People Arereallyannoyingshinkickers."},
 
 {DELAY,600,0},
 {DONE,0,0}
};
nogtext snow[]={
 {ICER,0,  "infooooma"},
 {ETHAN,0, "yaknow saydaddymesnow meuptoblame."}, 
 {STUMP,0,  "policemansay medaddymestump meshotsomeonedown thelaaane"},
 {ETHAN,0, "alickey boom boom down."}, 
 {ICER,0,  "Rap sucks."},
 {ETHAN,0, "I wish there was an alternative to rap."}, 
 {STUMP,0,  "There is, it's called Alternative."},
 {ETHAN,0, "I wish there was an Alternative to Alternative."}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

nogtext trap[]={
 {ICER,0,  "Man that clapper trap was mean, you really fell for it."},
 {ETHAN,0, "At least I didn't get eaten by a cow that you could plainly see Stumpy riding."}, 
 {ICER,0,  "How did you know that?"},
 {ETHAN,0, "Easy, I watched the credits cinematic at the beginning of the game."}, 
 {ICER,0,  "We are in a computer game?"},
 {ETHAN,0, "Yes, Stumpy must have trapped us here."}, 
 {STUMP,0,  "Shhhhhh...you are going to spoil the elaborate trap I have set for the player."},
 {ETHAN,0, "Run player!!!!! Run while you still can!!!!!"}, 
 
 {DELAY,600,0},
 {DONE,0,0}
};

//list of all conversations
nogtext *conversations[]=
 {
  intro, shutup, squat, notliketard, tirebiter, nipple, piss, bulge, underwear,
  cavity, bud, president, luger, easychair, captainplanet, bulldozers, jihad, shadow,
  beer, scooby, strode, car, wow, homepage, buddy, facesofdeath, cable, rimmer, grind,
  demon, retardation, peepee, chili, executioners, nn3, stars, tb, goldbond, astonish,
  hairpresident, beanbag, lolli, Review, goth, snow, trap,
  0
 };




nogtext win1[]={
 {ETHAN,0, "Hey, the person playing just won..."},
 {ETHAN,0, "I bet the girls are all falling at his or her feet right now."},
 {ICER,0, "I doubt it. All the girls are falling at Buddy's feet...he's such a stud."},
 {ETHAN,0, "You twit! The reason girls keep falling at Buddy's feet is because they keep slipping on his drool."},
 {ICER,0, "Buddy is a pimp!"},
 {ICER,0, "Buddy is the mack daddy."},
 {ETHAN,0, "You know why Buddy never gets a girl pregnant?"},
 {ICER,0, "Does he use protection?"},
 {ETHAN,0, "No, it's because he has an extra chromosome."},
 {ICER,0, "XXY!"},
 {ETHAN,0, "XXY!"},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext win2[]={
 {ETHAN,0, "Hey, the person playing just won..."},
 {ETHAN,0, "That means they receive the grand prize!"},
 {ICER,0, "Yes."},
 {ICER,0, "Ummm...I didn't know we gave away prizes?"},
 {ETHAN,0, "Well, we were going to give away a free skin illustration to every winner."},
 {ICER,0, "Skin illustration? You mean a tattoo?"},
 {ETHAN,0, "They are not tattoos! They are skin illustrations!!"},
 {ICER,0, "Hey hey...chill out. Tattoo illustrations are cool.."},
 {ETHAN,0, "They are NOT TATTOOS! THEY ARE SKIN ILLUSTRATIONS!!"},
 {ICER,0, "Oh."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext win3[]={
 {ETHAN,0, "Wow...we have a winner."},
 {ETHAN,0, "You must not have programmed the computer very well."},
 {ICER,0, "No it's like chess you see, I let them win this time."},
 {ICER,0, "That way I build up false psychological hope and crush it later."},
 {ETHAN,0, "So you are saying the player is a mere pawn in the big picture?"},
 {ICER,0, "Yes, I am dominating the player's life, crafting it to my very whims."},
 {ETHAN,0, "And soon you can make the player do anything you want?"},
 {ICER,0, "Yes, you see how the pieces are falling into place."},
 {ETHAN,0, "What if they just turn the game off?"},
 {ICER,0, "Shhhhhhhhhh!"},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext win4[]={
 {ETHAN,0, "Wow, the player is on a roll."},
 {ETHAN,0, "Maybe the player is a cool person after all!"},
 {ICER,0, "No way, think about what they are doing."},
 {ICER,0, "They are actually skilled at a crappy little pong game that we threw together."},
 {ETHAN,0, "True, they could spend their time better Milking Buffalo."},
 {ICER,0, "...or sending bombs in the mail to political officials."},
 {ETHAN,0, "...or having romance on internet chat programs with ugly chicks."},
 {ICER,0, "...or coveting thy neighboor's sheep."},
 {ETHAN,0, "...or making blue toilet water green."},
 {ICER,0, "...or waiting 20 seconds to see the really cool thing we'll say next."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

//list of win conversations
#define NUMWINCONV 4
nogtext *winconversations[]=
 {
   win1,win2,win3,win4
 };



nogtext lost[]={
 {ETHAN,0, "Hey, the person playing just lost..."},
 {ICER,0, "What a loser."},
 {ETHAN,0, "Go cry to momma!"},
 {ICER,0, "Momma's little baby. Waaaa waaaa."},
 {ETHAN,0, "Just end your misery and put a gun to your head..."},
 {ICER,0, "We have a problem in today's society with teenagers blowing their own brains out."},
 {ICER,0, "Such a waste..."},
 {ETHAN,0, "...a waste of a bullet."},
 {ICER,0, "Yeah, why can't they just slit their wrists like we did back in the old days?"},
 {ETHAN,0, "You can reuse knifes over and over. Recycling is the future."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext lost2[]={
 {ETHAN,0, "What, you lost again?"},
 {ICER,0, "Do you have limb control problems or something?"},
 {ETHAN,0, "Maybe the person playing is having an epileptic fit."},
 {ICER,0, "The flashy lights can do that to you."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext lost3[]={
 {ETHAN,0, "If you keep losing like that, you'll never see the spiffy endings."},
 {ICER,0, "Endings? There are endings in this game?"},
 {ETHAN,0, "Yeah you tosser, we made this game remember? You should know!"},
 {ICER,0, "I didn't know this game even ended, I thought we were trapped here forever."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};

nogtext lost4[]={
 {ETHAN,0, "Tsk tsk tsk..."},
 {ICER,0, "You naughty boy you."},
 {ETHAN,0, "You're supposed to hit the head back each time...didn't you read the instructions?"},
 {ICER,0, "Yeah, we spent a whole 5 minutes writing the instructions and you still miss the head."},
 
 {DELAY,2000,0}, 
 {ICER,0,  "Press the damn button already!"},
 {ETHAN,0, "Hurry your ass up, we're getting bored."},
 {DONE,0,0}
};


nogtext lost5[]={
 {ETHAN,0, "Have faith, we know that winning this game is one of your ambitions."},
 {ICER,0, "Yes, don't let herpes get in the way of your dreams..."},
 {ETHAN,0, "Herpes thrives on ignorance."},
 
 {DONE,0,0}
};

nogtext endingtext[]={
 {ETHAN,0, "Well, finally."},
 {ICER,0, "Yeah, we've been waiting forever."},
 {ICER,0, "My ass feels like it's asleep."},
 {ICER,0, "You beat all the characters."},
 {ETHAN,0, "Yes, you have beaten them all..."},
 {ICER,0, "You've knocked the noggin quite successfully."},
 {ETHAN,0, "Now is the time where you get to see the ending."},
 {ICER,0, "Yeah, so, let's fire that ending up..."},
 {ICER,0, "Okay, any moment now..."},
 {ICER,0, "Hey wait, you need to press the button first."},
 {ETHAN,0, "Yeah, press the damn button so we can get the hell outta here."},
 {DONE,0,0}
};

nogtext *endingtextptr=endingtext;

//list of lose conversations
#define NUMLOSECONV 5
nogtext *loseconversations[]=
 {
   lost, lost2, lost3, lost4, lost5
 };



nogtext donetext[]=
{
  {DELAY,100,0},
  {DELAY,100,0},
  {DONE,0,0},
};




//position of conversation
point textpos[]={
 {0,0},   //done
 {100,140}, //icer's position
 {220,140}, //ethan's position
 {160,35}, //stumps's position
};
int textwidth[]={0,180,180,320-100};


//current conversation
nogtext **convptr; //current conversation
nogtext *textptr; //current text being spoken
int texttimeout;  //time left to show text
int person;       //person speaking
int parsetext;    //parse new text
char textline[10][50];    //lines shown on screen
int textcenter[10];
char *textdest;
int numconv,currconv,numconvdone; //number of conversations
char convdone[50]; //whether conversation has been used yet or not



 
