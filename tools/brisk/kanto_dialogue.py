"""Local subjects and safe source text for Kanto's Champion visit.

Keep the imported residents' personalities where their dialogue is independent
of FRLG's badge campaign. Do not execute any imported event script.
"""
import re

# A decade of recovery after Team Rocket's dissolution explains the
# postgame region's unusually high wild and trainer Pokemon levels. Each
# line belongs to one specific resident, not every generic NPC.
ROCKET_ERA_RESIDENTS = {
    ("PalletTown_Frlg", 3): "Ten years ago, Team Rocket dissolved. Professor Oak says Kanto's Pokemon have had a decade to grow undisturbed.",
    ("Route1_Frlg", 2): "The wild Pokemon here seem tough, don't they? Ten years without Team Rocket's interference gave them time to grow strong.",
    ("ViridianCity_Frlg", 4): "Since Team Rocket broke apart ten years ago, Kanto's Trainers have trained in peace. Even the roads to Viridian are tougher now.",
    ("ViridianForest_Frlg", 6): "Team Rocket stopped hunting Pokemon here a decade ago. Now the forest's Pokemon are stronger than ever!",
    ("PewterCity_Frlg", 3): "Pewter isn't just for beginners anymore. Ten years after Team Rocket fell, our Trainers have become serious challengers.",
    ("CeruleanCity_Frlg", 1): "Misty says Kanto's wild Pokemon grew strong during ten peaceful years after Team Rocket's collapse. Her team did, too.",
    ("VermilionCity_Frlg", 5): "Sailors who return after ten years barely recognize our battles. With Rocket gone, everyone kept raising their teams.",
    ("LavenderTown_Frlg", 3): "After Team Rocket dissolved, Pokemon here could finally grow without fear. Strength can come from kindness, too.",
    ("CeladonCity_Frlg", 1): "The old Team Rocket hideout is history now. We've used these ten quiet years to train and build a stronger city.",
    ("SaffronCity_Frlg", 2): "Silph rebuilt after Team Rocket's fall. Ten years of new training methods made our city's Pokemon formidable.",
    ("FuchsiaCity_Frlg", 2): "A decade free from Rocket poachers let Kanto's habitats recover. These wild Pokemon can challenge even veterans.",
    ("CinnabarIsland_Frlg", 2): "Our researchers have measured stronger wild Pokemon across Kanto. Ten years of peace made a real difference.",
    ("Route15_Frlg", 11): "Ever since Team Rocket disbanded ten years ago, Route 15 has been a training ground for experienced teams.",
    ("VictoryRoad_1F_Frlg", 3): "Ten years of uninterrupted training have made Victory Road tougher than ever. Even visiting Champions must prepare.",
}

ROCKET_ERA_BOSS_INTROS = {
    "TRAINER_LEADER_BROCK": "BROCK: Ten years since Rocket collapsed, Kanto's Pokemon and Trainers have grown stronger. Let's show Hoenn's Champion just how much!",
    "TRAINER_LEADER_MISTY": "MISTY: Kanto's waters had ten peaceful years to flourish after Rocket's fall. These Pokemon won't go easy on a Champion!",
    "TRAINER_ELITE_FOUR_LANCE": "LANCE: Team Rocket dissolved a decade ago. Kanto used that peace to forge exceptional teams. Show us what Hoenn's savior can do!",
}

TOPICS = {
 'PalletTown': ["The sea breeze carries all the way up Route 1.", "Oak's research started with the Pokemon living right outside town.", "Small towns send Trainers on very long journeys.", "I like watching travelers return with partners from distant regions.", "From here, the southern sea leads toward Cinnabar.", "Our quiet mornings are worth coming home for."],
 'Viridian': ["The forest paths are cool even at midday.", "Route 22 attracts Trainers preparing for Victory Road.", "The Gym draws visitors from all over Kanto.", "I walk north before the forest gets busy.", "The pond is my favorite place to rest after training.", "I always pack supplies before heading toward Pewter."],
 'Pewter': ["The museum makes ancient Pokemon feel close enough to touch.", "Brock learned to be patient from Rock Pokemon.", "Mt. Moon is just beyond the eastern route.", "A fossil can hold a whole chapter of our world's history.", "The mountain paths reward careful preparation.", "I listen for Clefairy when the moon is bright."],
 'Cerulean': ["The cape is lovely after the bridge crowds leave.", "Misty's pool is a training ground, not a place to paddle.", "The northern paths are good places to practice doubles.", "Water Pokemon thrive along these riverbanks.", "I follow the river when I need to clear my head.", "A fast team still needs a plan for bulky opponents."],
 'Vermilion': ["Watching ships leave makes me want to travel again.", "Surge's Electric Pokemon make short work of careless Water teams.", "The harbor changes with every tide.", "Diglett's Cave is a very different journey from the coastal road.", "I can recognize visiting sailors by their Pokemon.", "Salt gets into everything near the docks."],
 'Lavender': ["The tower is a place to remember the partners we have lost.", "Mr. Fuji has a kind word for every Pokemon.", "I leave flowers here when I visit old friends.", "Please keep your voice down near the memorials.", "Some friendships stay with us long after a journey ends.", "Caring for a Pokemon matters more than collecting trophies."],
 'Celadon': ["The department store is easy to enter and hard to leave.", "Erika's garden smells different after the rain.", "The rooftop is my escape from the shopping crowds.", "I come here for the plants as much as the shops.", "A quiet afternoon can be more valuable than a big purchase.", "The city's little alleys hide some lovely places to rest."],
 'Fuchsia': ["The wildlife here deserves room to roam.", "Koga teaches patience as well as battle technique.", "The southern coast is a demanding swim.", "I prefer watching Pokemon without disturbing them.", "There is always another call to hear beyond the trees.", "An unfamiliar Pokemon is a reason to observe carefully."],
 'Saffron': ["Silph's technology connects people across regions.", "Sabrina's calm is harder to copy than her Psychic techniques.", "The dojo and the Gym offer very different lessons.", "The crossroads keep this city moving.", "I take the long way home to avoid the busiest streets.", "Even in a big city, Pokemon need somewhere quiet to rest."],
 'Cinnabar': ["The lab studies Pokemon that most people know only as fossils.", "Blaine's questions are easier than his battles.", "The mansion is no place to wander without a healthy team.", "Seafoam's cold water meets our warm coastal currents.", "The eastern sea connects us with the Orange Islands.", "Our island is small, but its history runs deep."],
 'PokemonLeague': ["Victory Road demands more than raw power.", "I check every held item before a serious challenge.", "A strong opening means little without a way to finish.", "The best teams can recover when their first plan fails.", "Getting this far takes trust between a Trainer and their Pokemon.", "I watch League battles to study the switches between attacks."],
}
GENERAL = ["I keep a notebook of the Pokemon I meet on my travels.", "My partner notices changes in the weather before I do.", "We take a break whenever one of us gets tired.", "I prefer finding my own route to following a crowd.", "I still learn something from battles I lose.", "A long journey gives you time to understand your Pokemon."]
PERSONAL = [
 "I used to rush everywhere. My partner taught me to slow down.",
 "I brought extra food today. My Pokemon never forget snack time.",
 "I promised myself I would explore somewhere new this week.",
 "I get nervous before battles, even after all this practice.",
 "I would rather train one reliable partner than chase every new trend.",
 "I keep a place in my team for a Pokemon I haven't met yet.",
 "My family asks for postcards. I always end up drawing Pokemon on them.",
 "I came out for a short walk and stayed until sunset last time.",
 "My partner likes the journey more than the destination.",
 "I remember my first loss more clearly than my first win.",
 "I practice switching as much as attacking.",
 "One well-timed status move saved my last battle.",
 "I thought I knew this place until I explored it with my Pokemon.",
 "My favorite victories are the ones my whole team helped earn.",
 "I packed for a long trip. My partner seems ready for an even longer one.",
 "I take a different path each time I come here.",
]
NAMED = {
 'BILL': "BILL: You're Hoenn's Champion! I'm here comparing Kanto's fossil research with Pokemon storage records. A new region means plenty of new partners to learn about.",
 'DAISY': "DAISY: Traveling all the way from Hoenn must have been tiring. A Champion's Pokemon deserve a little quiet time together, too.",
 'MR_FUJI': "MR. FUJI: You helped protect Hoenn's people and Pokemon. Please carry that same kindness with you here. Every partner deserves to feel safe.",
 'CELIO': "CELIO: BILL told me about Hoenn's Champion. I'm studying how Trainers stay connected across the islands. Distance should never stop people sharing what they learn.",
}
# These phrases depend on campaigns, rewards, or services that are disabled here.
UNSAFE = re.compile(r"rocket|badge|rival|\boak\b|\bblue\b|\bred\b|\bmay\b|\bbrendan\b|\bchampion\b|\bhoenn\b|\bsavior\b|\bgiovanni\b|\bbill\b|\bcelio\b|\bfuji\b|\bdaisy\b|\bHM\d*\b|\bTM\d*\b|ferry|seagallop|ticket|\bcut\b|\bstrength\b|flash|teach|give you|take this|take these|received|parcel|deliver|silph scope|poke flute|pokeflute|snorlax|saffron.*guard|thirsty|drink|trade|buy|sell|coin|prize|safari.*game|500|lostelle|lorelei|password|key|card|defeat.*league|challeng.*league|become.*champion",re.I)


def place(name):
    raw=name.removesuffix('_Frlg').split('_')[0]
    return re.sub(r'(?<=[a-z])(?=[A-Z0-9])',' ',raw).replace('Pokemon','Pokemon ')


def source_words(label, scripts):
    """Extract text only from a simple direct message, never run old scripts."""
    body=scripts.get(label,'')
    match=re.search(r'\bmsgbox\s+(\w+)',body)
    if not match:return None
    quoted=re.findall(r'\.string "(.*)"',scripts.get(match[1],''))
    if not quoted:return None
    words=''.join(quoted)
    words=re.sub(r'\\[nlp]',' ',words).replace('$','')
    words=re.sub(r'\s+',' ',words).strip()
    # Control codes and dynamic buffers need their original script preparation.
    if not words or any(x in words for x in ('{','}','\\','"')) or UNSAFE.search(words):return None
    return words


class Dialogue:
    def __init__(self):
        self.used=set()
        self.counts={}

    def unique(self, words, name, role='resident'):
        if words and words not in self.used:
            self.used.add(words);return words
        topics=next((v for k,v in TOPICS.items() if name.startswith(k)),[f'Here in {place(name)}, '+line[0].lower()+line[1:] for line in GENERAL])
        key=(name,role);number=self.counts.get(key,0);self.counts[key]=number+1
        # Route subjects describe the actual habitat rather than a generic city.
        route=re.match(r'Route(\d+)',name)
        if route:
            n=int(route[1]);topics=[
                f"Route {n} has become a training ground for experienced teams.",
                f"I record the wild Pokemon living along Route {n}.",
                f"I always heal before another trip along Route {n}.",
                f"Route {n} looks familiar, but its wild Pokemon still surprise me.",
                f"I've spent whole afternoons exploring Route {n} with my partner.",
                f"The Trainers on Route {n} take preparation seriously."]
        if 'PokemonCenter' in name:topics=[f"This is my rest stop in {place(name)}.",f"I compare training notes with visitors to {place(name)}.",f"I reached {place(name)} with my last Pokemon barely standing."]+topics
        if 'Mart' in name or 'DepartmentStore' in name:topics=[f"I came to {place(name)} to restock before my next journey.",f"I check my Bag before shopping in {place(name)}."]+topics
        if 'SCIENTIST' in role:topics=[f"I'm observing how Pokemon adapt around {place(name)}.",f"Our field notes from {place(name)} still have unanswered questions."]+topics
        for offset in range(4096):
            i=number+offset
            words=topics[i%len(topics)]+' '+PERSONAL[(i//len(topics))%len(PERSONAL)]
            cycle=i//(len(topics)*len(PERSONAL))
            if cycle:words+=' '+PERSONAL[(cycle+3)%len(PERSONAL)]
            if words not in self.used:self.used.add(words);return words
        raise ValueError(('Unwritten unique dialogue',name,role))

BATTLE_LINES = {
 'intro': ["I've been training for a visitor as strong as you. Ready?", "A Champion still has to read the opponent. Let's battle!", "I'll put everything I've practiced into this battle.", "Let's see how our teams handle a real challenge.", "I've saved my best partners for a battle like this.", "You saved Hoenn. Now let's see the teamwork behind that title!", "My team can turn one opening into a victory. Watch closely!", "I won't hold back just because you're visiting from Hoenn.", "I've prepared more than one way to win this.", "Let's give our Pokemon a battle worth remembering!", "Your reputation reached Kanto before you did. My team's ready!", "I want to see how a Champion handles my strategy."],
 'defeat': ["You found the opening before I did!", "That last exchange decided everything.", "My team gave everything it had. Well battled!", "I needed a better answer to that switch.", "You kept your plan together under pressure.", "I made one prediction too many!", "Now I see how you earned that title.", "That was a lesson I couldn't learn from watching.", "You made every Pokemon on your team count.", "I couldn't recover once you took control.", "I almost had an opening. You closed it quickly!", "You've given me something new to practice."],
 'after': ["Next time, I'll prepare for the strategy you showed me.", "I'm going over our switches before our next training session.", "My partners earned a rest after that effort.", "Battling a visiting Champion gives us a fresh way to think.", "I need to practice recovering when my opening plan fails.", "I want our next victory to come from better teamwork.", "The loss stings, but I know what to work on now.", "I'll remember how patiently you waited for the right moment.", "I'm glad my Pokemon got to face a team like yours.", "I'll tell my training partners what I learned today.", "That battle made the journey here worthwhile.", "I hope Kanto keeps giving your team good challenges."]
}


def battle_words(dialogue, scripts, body, script, name, trainer, phase, seed):
    labels=re.search(r'trainerbattle_\w+\s+TRAINER_\w+,\s*(\w+),\s*(\w+)',body)
    old=source_words(script,scripts) if phase=='after' else None
    if labels and phase in ('intro','defeat'):
        old=source_words('_direct', {**scripts,'_direct':'msgbox '+labels[1 if phase=='intro' else 2]})
    if old and old not in dialogue.used:
        dialogue.used.add(old);return old
    for i in range(len(BATTLE_LINES[phase])):
        phrase=BATTLE_LINES[phase][(seed+i)%len(BATTLE_LINES[phase])]
        words=f"{trainer}: {phrase}"
        if words not in dialogue.used:dialogue.used.add(words);return words
    # Repeated names can be separate people on different routes.
    words=f"{trainer}: Here in {place(name)}, {BATTLE_LINES[phase][seed%len(BATTLE_LINES[phase])][0].lower()+BATTLE_LINES[phase][seed%len(BATTLE_LINES[phase])][1:]}"
    return dialogue.unique(words,name,phase)
