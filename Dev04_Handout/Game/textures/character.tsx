<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.5" name="character" tilewidth="50" tileheight="37" tilecount="96" columns="8">
 <image source="character.png" width="400" height="444"/>
 <tile id="90">
  <properties>
   <property name="name" value="Death"/>
   <property name="total_frames" type="int" value="7"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="-102" y="401"/>
  </objectgroup>
  <animation>
   <frame tileid="62" duration="150"/>
   <frame tileid="63" duration="150"/>
   <frame tileid="64" duration="150"/>
   <frame tileid="65" duration="150"/>
   <frame tileid="66" duration="200"/>
   <frame tileid="67" duration="200"/>
   <frame tileid="68" duration="500"/>
  </animation>
 </tile>
 <tile id="91">
  <properties>
   <property name="name" value="Slide"/>
   <property name="total_frames" type="int" value="7"/>
  </properties>
  <animation>
   <frame tileid="14" duration="150"/>
   <frame tileid="15" duration="150"/>
   <frame tileid="24" duration="150"/>
   <frame tileid="25" duration="150"/>
   <frame tileid="26" duration="150"/>
   <frame tileid="27" duration="150"/>
   <frame tileid="28" duration="150"/>
  </animation>
 </tile>
 <tile id="92">
  <properties>
   <property name="name" value="Jump"/>
   <property name="total_frames" type="int" value="8"/>
  </properties>
  <animation>
   <frame tileid="14" duration="150"/>
   <frame tileid="15" duration="150"/>
   <frame tileid="16" duration="150"/>
   <frame tileid="17" duration="150"/>
   <frame tileid="22" duration="150"/>
   <frame tileid="23" duration="150"/>
   <frame tileid="22" duration="150"/>
   <frame tileid="23" duration="150"/>
  </animation>
 </tile>
 <tile id="93">
  <properties>
   <property name="name" value="Walk"/>
   <property name="total_frames" type="int" value="6"/>
  </properties>
  <animation>
   <frame tileid="8" duration="150"/>
   <frame tileid="9" duration="150"/>
   <frame tileid="10" duration="150"/>
   <frame tileid="11" duration="150"/>
   <frame tileid="12" duration="150"/>
   <frame tileid="13" duration="150"/>
  </animation>
 </tile>
 <tile id="94">
  <properties>
   <property name="name" value="Crouch"/>
   <property name="total_frames" type="int" value="4"/>
  </properties>
  <animation>
   <frame tileid="4" duration="200"/>
   <frame tileid="5" duration="200"/>
   <frame tileid="6" duration="200"/>
   <frame tileid="7" duration="200"/>
  </animation>
 </tile>
 <tile id="95">
  <properties>
   <property name="name" value="Idle"/>
   <property name="total_frames" type="int" value="4"/>
  </properties>
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="200"/>
   <frame tileid="2" duration="200"/>
   <frame tileid="3" duration="200"/>
  </animation>
 </tile>
</tileset>
